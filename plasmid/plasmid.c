

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
		return JVMTI_ERROR_NULL_POINTER ;
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

jvmtiError getMethodInfo(jvmtiEnv *jvmtiEnv, jthread thread, jvmtiFrameInfo* jvmtiFrame, char **methodName, char **signature) 
{
	if (jvmtiEnv == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER ;
	}
	
	return (*jvmti)->GetMethodName(jvmti, (*jvmtiFrame).method, methodName, signature, NULL);
}

jvmtiError getMethodArgumentCount(jvmtiEnv *jvmtiEnv, jmethodID method, jint* argumentCount)
{
	if (jvmtiEnv == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER ;
	}
	
	return (*jvmti)->GetArgumentsSize(jvmti, method, argumentCount);
}

jvmtiError getLineNumberTable(jvmtiEnv* jvmtiEnv, jvmtiFrameInfo* frameInfo, jvmtiLineNumberEntry** lineNumberTableEntry, jint** lineNumberTableSize)
{
	if (jvmtiEnv == NULL || frameInfo == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER ;
	}
	
	return (*jvmti)->GetLineNumberTable(jvmtiEnv, (*frameInfo).method, lineNumberTableEntry, lineNumberTableSize);
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
	if (!hasClassLoader(jvmtiEnv, method)) 
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
	
	jint index = 0;
	for (index = 0; index < frameCount; index++)
	{
		PDEBUG("\t * frame location : %d\n", frameInfo[index].location);
	}
	
	for (index = 0; index < frameCount; index++)
	{
		char* methodName = NULL;
		char* signature = NULL;
		
		if (getMethodInfo(jvmtiEnv, thread, &frameInfo[index], &methodName, &signature) != JVMTI_ERROR_NONE)
		{
			SAFE_FREE(methodName);
			SAFE_FREE(signature);
			continue;
		}
		
		
		char* classSignature = NULL;
		if (getClassSignature(jvmtiEnv, frameInfo[index].method, &classSignature) != JVMTI_ERROR_NONE)
		{			
			SAFE_FREE(methodName);
			SAFE_FREE(signature);
			continue;
		}
		
		
		PDEBUG("\t\t * Executing method: %s in class %s(location : %d) signature(%s)\n", methodName, classSignature, frameInfo[index].location, signature);
		
		jint lineNumberTableSize;
		jvmtiLineNumberEntry* lineNumberTableEntry;
		
		if (getLineNumberTable(jvmtiEnv, &frameInfo[index], &lineNumberTableSize, &lineNumberTableEntry) != JVMTI_ERROR_NONE)
		{
			SAFE_FREE(classSignature);
			continue;
		}
		
		jint i = 0;
		jint codeLineNumber = 0;
		for (i = 0; i < lineNumberTableSize; i++)
		{
			PDEBUG("\t\t\t lineNumber index(%d) %d to %d\n", i, lineNumberTableEntry[i].start_location, lineNumberTableEntry[i].line_number);
		
			if (frameInfo[index].location >= lineNumberTableEntry[i].start_location) 
			{
				codeLineNumber = lineNumberTableEntry[i].line_number;
			}
		}
		
		PDEBUG("\t\t\t\t java code line number(%d)\n", codeLineNumber);
		doRecord(logger, "\t*execute method(%s) line(%d) in class(%s)\n", methodName, codeLineNumber, classSignature);
		
		jint methodArgumentCount = 0;
		if (getMethodArgumentCount(jvmtiEnv, frameInfo[index].method, &methodArgumentCount) == JVMTI_ERROR_NONE)
		{
			PDEBUG("\t\t\t\t argumentCount(%d)\n", methodArgumentCount);
		}
			
		SignatureCode* signatureCodes;
		jint signatureCodeCount = 0;
		if (parseMethodSignature(signature, methodArgumentCount, &signatureCodes, &signatureCodeCount) == JVMTI_ERROR_NONE && signatureCodeCount > 0) 
		{
			jint index = 0;
			PDEBUG("\t\t\t\t method argument signature\n");
			for (index = 0; index < methodArgumentCount; index++)
			{
				if (signatureCodes[index] == _undefined)
				{
					continue;
				}
				
				SignatureValue* signatureValue;

				if (getValue(jvmtiEnv, jniEnv, thread, index, signatureCodes[index], &signatureValue) != JVMTI_ERROR_NONE)
				{
					SAFE_FREE(signatureValue->value);
					SAFE_FREE(signatureValue);
					continue;
				}
				
				printSignatureValue(signatureValue, logger);
				
				SAFE_FREE(signatureValue->value);
				SAFE_FREE(signatureValue);
			}
			SAFE_FREE(signatureCodes);
		}
		
		SAFE_FREE(classSignature);
		SAFE_FREE(lineNumberTableEntry);
		
		SAFE_FREE(methodName);
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

	/*
		#define JNI_OK           0                 // success 
		#define JNI_ERR          (-1)              // unknown error 
		#define JNI_EDETACHED    (-2)              // thread detached from the VM
		#define JNI_EVERSION     (-3)              // JNI version error
		#define JNI_ENOMEM       (-4)              // not enough memory
		#define JNI_EEXIST       (-5)              // VM already created
		#define JNI_EINVAL       (-6)              // invalid arguments
	*/
	jint result = (*jvm)->GetEnv(jvm, (void **) &jvmti, JVMTI_VERSION);
	if (result != JNI_OK) {
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
