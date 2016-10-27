

#include "plasmid.h"

jobject last_exception;
 
jvmtiError getFrameCount(jvmtiEnv *jvmtiEnv, jthread thread, jint* frameCount)
{
	return (*jvmti)->GetFrameCount(jvmtiEnv, thread, frameCount);
}

jvmtiError getStackTrace(jvmtiEnv *jvmtiEnv, jthread thread, jvmtiFrameInfo* frameInfo, int maxFrameCount, jint* frameCount)
{
	if (jvmtiEnv == NULL || frameInfo == NULL || frameCount == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER;
	}
	
	/*
		startDepth
		if zero, start from the current frame; 
		if one, start from the caller of the current frame; 
		if two, start from the caller of the caller of the current frame; and so on
	*/
	jint startDepth = 0;
	
	return (*jvmti)->GetStackTrace(jvmtiEnv, thread, startDepth, maxFrameCount, frameInfo, frameCount);
}

jvmtiError getMethodInfo(jvmtiEnv *jvmtiEnv, jthread thread, jmethodID method, char **methodName, char **signature) 
{
	if (jvmtiEnv == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER;
	}
	
	return (*jvmti)->GetMethodName(jvmti, method, methodName, signature, NULL);
}

jvmtiError getMethodArgumentCount(jvmtiEnv *jvmtiEnv, jmethodID method, jint* argumentCount)
{
	if (jvmtiEnv == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER;
	}
	
	return (*jvmti)->GetArgumentsSize(jvmti, method, argumentCount);
}

jvmtiError getLineNumberTable(jvmtiEnv* jvmtiEnv, jmethodID method, jvmtiLineNumberEntry** lineNumberTableEntry, jint** lineNumberTableSize)
{
	if (jvmtiEnv == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER;
	}
	
	return (*jvmti)->GetLineNumberTable(jvmtiEnv, method, lineNumberTableEntry, lineNumberTableSize);
}

jvmtiError getLineNumber(jvmtiLineNumberEntry* lineNumberTableEntry, jint lineNumberTableSize, jvmtiFrameInfo* jvmtiFrame, jint* codeLineNumber)
{
	if (lineNumberTableEntry == NULL || lineNumberTableSize <= 0 || jvmtiFrame == NULL || codeLineNumber == NULL)
	{
		return JVMTI_ERROR_NULL_POINTER;	
	}
	
	jint index = 0;
	for (index = 0; index < lineNumberTableSize; index++)
	{
		if ((*jvmtiFrame).location >= lineNumberTableEntry[index].start_location) 
		{
			*codeLineNumber = lineNumberTableEntry[index].line_number;
		}
	}
	
	return JVMTI_ERROR_NONE;
}

void JNICALL vmInitCallBack(jvmtiEnv *jvmtiEnv, JNIEnv *jniEnv, jthread thread) 
{
	PDEBUG("Agent::vm_init\n");
	
	initMethodIdTable(jniEnv);
	
	PDEBUG("MethodId Table\n");
	PDEBUG("\tmethodIdTable.toString[%d]\n", methodIdTable.toString);
	PDEBUG("\tmethodIdTable.intValue[%d]\n", methodIdTable.intValue);
	PDEBUG("\tmethodIdTable.longValue[%d]\n", methodIdTable.longValue);
	PDEBUG("\tmethodIdTable.floatValue[%d]\n", methodIdTable.floatValue);
	PDEBUG("\tmethodIdTable.doubleValue[%d]\n", methodIdTable.doubleValue);
}


void JNICALL vmDeathCallBack(jvmtiEnv *jvmtiEnv, JNIEnv *env) 
{
    PDEBUG("Agent::vm_death- Does this happen\n");
}

void JNICALL exceptionCallBack(jvmtiEnv *jvmtiEnv, 
							JNIEnv* jniEnv, 
							jthread thread, 
							jmethodID method, 
							jlocation location, 
							jobject exception, 
							jmethodID catchMethod, 
							jlocation catchLocation)
{
	
	jclass clazz;
	if (getClass(jvmtiEnv, method, &clazz) != JVMTI_ERROR_NONE) 
	{
		return;
	}
	
	if (!hasClassLoader(jvmtiEnv, clazz, method)) 
	{
		return;
	}
		
	char* exceptionSignature;
	if (getObjectSignature(jvmtiEnv, jniEnv, exception, &exceptionSignature) != JVMTI_ERROR_NONE) 
	{
		PDEBUG("cb_Exception (exception=%p)\n", exception);
		return;
	}
	
	Logger* logger;
	
	startRecord(&logger, exceptionSignature);
	
	
	PDEBUG("cb_Exception (exception=%p) / %s\n", exception, exceptionSignature);
	SAFE_FREE(exceptionSignature);
	
	/*
		typedef struct {
			jmethodID method;		// The method executing in this frame.
			jlocation location;		// The index of the instruction executing in this frame. -1 if the frame is executing a native method.
		} jvmtiFrameInfo;
	*/
	jvmtiFrameInfo frameInfo[STACK_FRAME_MAX_COUNT];
	jint frameCount = 0;
	
	if (getStackTrace(jvmtiEnv, thread, frameInfo, STACK_FRAME_MAX_COUNT, &frameCount) != JVMTI_ERROR_NONE) 
	{
		endRecord(&logger);
		return;
	}
	
	jint frameDepth = 0;
	for (frameDepth = 0; frameDepth < frameCount; frameDepth++)
	{
		char* methodName = NULL;
		char* signature = NULL;
		
		if (getMethodInfo(jvmtiEnv, thread, frameInfo[frameDepth].method, &methodName, &signature) != JVMTI_ERROR_NONE)
		{
			SAFE_FREE(methodName);
			SAFE_FREE(signature);
			continue;
		}
		
		
		char* classSignature = NULL;
		if (getClassSignature(jvmtiEnv, frameInfo[frameDepth].method, &classSignature) != JVMTI_ERROR_NONE)
		{			
			SAFE_FREE(methodName);
			SAFE_FREE(signature);
			continue;
		}
		
		PDEBUG("\t\t*Executing method: %s in class %s(location : %d) signature(%s)\n", methodName, classSignature, frameInfo[frameDepth].location, signature);
		
		jint lineNumberTableSize;
		jvmtiLineNumberEntry* lineNumberTableEntry;
		
		if (getLineNumberTable(jvmtiEnv, frameInfo[frameDepth].method, &lineNumberTableSize, &lineNumberTableEntry) != JVMTI_ERROR_NONE)
		{
			SAFE_FREE(classSignature);
			continue;
		}
		
		jint codeLineNumber = 0;
		if (getLineNumber(lineNumberTableEntry, lineNumberTableSize, &frameInfo[frameDepth], &codeLineNumber)== JVMTI_ERROR_NONE)
		{
			PDEBUG("\t\t\t\t java code line number(%d)\n", codeLineNumber);
		}

		doRecord(logger, "\n\t*at method(%s:%d) in class(%s)\n", methodName, codeLineNumber, classSignature);
		
		SAFE_FREE(classSignature);
		SAFE_FREE(methodName);
		
		jint methodArgumentCount = 0;
		if (getMethodArgumentCount(jvmtiEnv, frameInfo[frameDepth].method, &methodArgumentCount) == JVMTI_ERROR_NONE)
		{
			PDEBUG("\t\t\t\t argumentCount(%d)\n", methodArgumentCount);
		}
		
		SAFE_FREE(lineNumberTableEntry);
		
		
		Value* values;
		initValues(&values, methodArgumentCount, signature);
		KDEBUG("@ after init values argumentCount(%d)\n", methodArgumentCount);
		
		if (getValue(jvmtiEnv, jniEnv, thread, frameDepth, values, methodArgumentCount) != JVMTI_ERROR_NONE) 
		{
			destroyValues(values, methodArgumentCount);
			SAFE_FREE(signature);
			
			continue;
		}
		
		int index = 0;
		for (index = 0; index < methodArgumentCount; index++)
		{
			values[index].printValue(logger, values[index].data);
		}
		
		destroyValues(values, methodArgumentCount);
		
		SAFE_FREE(signature);
	}

	
	endRecord(&logger);
		
	last_exception = exception;
}


void JNICALL exceptionCatchCallBack (jvmtiEnv *jvmtiEnv,
		    JNIEnv* jniEnv, jthread thread, jmethodID method, jlocation location,
			    jobject exception)
{
	
	PDEBUG ("cb_ExceptionCatch (exception=%p vs last_exception=%p)\n"
		"AreSameObject? = %d \n",
		exception,
		last_exception,
		(*jniEnv)->IsSameObject(jniEnv, exception, last_exception)
		);
}


JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved)
{
	PDEBUG("JVMTI Hello [%s]\n", options);
	
	logLocation = options;
	
	jvmtiCapabilities capabilities;
	jvmtiEventCallbacks callbacks;

	jint result = (*jvm)->GetEnv(jvm, (void **) &jvmti, JVMTI_VERSION);
	if (result != JNI_OK) 
	{
		PDEBUG("ERROR: Unable to access JVMTI!\n");
		return JNI_ERR;
	}

	memset(&capabilities, 0, sizeof(capabilities));
	capabilities.can_generate_exception_events = 1;
	capabilities.can_access_local_variables = 1;
	capabilities.can_get_line_numbers = 1;
	capabilities.can_get_source_file_name = 1;
	
	jvmtiError error = (*jvmti)->AddCapabilities(jvmti, &capabilities);

	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.Exception = &exceptionCallBack;
	callbacks.ExceptionCatch = &exceptionCatchCallBack;
	callbacks.VMInit = &vmInitCallBack;
	callbacks.VMDeath = &vmDeathCallBack;
	
	(*jvmti)->SetEventCallbacks(jvmti, &callbacks, sizeof(callbacks));

	(*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, NULL);
	(*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION_CATCH, NULL);
	(*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, NULL);
	(*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_VM_DEATH, NULL);
	
	return JNI_OK;
}
