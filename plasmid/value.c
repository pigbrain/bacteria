
#include "value.h"


jvmtiError initValues(Value** values, jint argumentCount, char* signature)
{
	KDEBUG("@ init values argumentCount(%d) signature(%s)\n", argumentCount, signature);
	
	*values = malloc(sizeof(Value) * argumentCount);
	
	int index = 0;
	char* p = signature;
	while (*p)
	{
		if (*p == SIGNATURE_START)
		{
			p++;
			continue;
		}
		
		if (*p == SIGNATURE_END)
		{
			break;
		}

		Value* position = (*values) + index;
		memset(position, 0, sizeof(Value));
		position->dataType = undefined;
		
		_parseSignature(position, &p);
		
		index++;
	}
	
	return JVMTI_ERROR_NONE;
}

jvmtiError destroyValues(Value* values, jint argumentCount)
{
	KDEBUG("@ destroy values argumentCount(%d)\n", argumentCount);
	
	int index = 0;
	for (index = 0; index < argumentCount; index++)
	{
		SAFE_FREE(values[index].data);
	}
	
	SAFE_FREE(values);
	
	return JVMTI_ERROR_NONE;
}

bool _isReference(char** signature)
{
	char* p = *signature;
	
	return (*p == SIGNATURE_REFERENCE_PREFIX);
}

bool _isArray(char** signature) 
{
	char* p = *signature;
	
	return (*p == SIGNATURE_array);
}

jvmtiError _parseSignature(Value* value, char** signature)
{
	bool isArray = _isArray(signature);
	if (isArray)
	{
		(*signature)++; // [
		
		value->dataType = array;
	}
	
	if (_isReference(signature))
	{
		KDEBUG("\t* signature(%c) is reference\n", **signature);
		
		(*signature)++; // prefix 'L'
		
		value->dataType = value->dataType == undefined ? reference : value->dataType;
		
		_parseReferenceSignature(value, signature);
		
		(*signature)++; // postfix ';'
	}
	else 
	{
		KDEBUG("\t* signature(%c) is primitive\n", **signature);
		
		value->dataType = value->dataType == undefined ? primitive : value->dataType;
		
		_parsePrimitiveSignature(value, signature);
	}
	
	if (isArray)
	{
		
		value->getValue = _getReferenceObjectValue;
		value->printValue = _printReferenceObjectValue;
	}
	
	return JVMTI_ERROR_NONE;
}

jvmtiError _parsePrimitiveSignature(Value* value, char** signature)
{
	char* p = *signature;
	(*signature)++;

	if (*p == SIGNATURE_int)
	{
		KDEBUG("\t* SIGNATURE_int\n");
		
		value->signature[0] = SIGNATURE_int;
		value->getValue = _getPrimitiveIntValue;
		value->printValue = _printPrimitivieIntValue;
	}
	else if (*p == SIGNATURE_long)
	{
		KDEBUG("\t* SIGNATURE_long\n");
		
		value->signature[0] = SIGNATURE_long;
		value->getValue = _getPrimitiveIntValue;
		value->printValue = _printPrimitivieIntValue;
	}
	else if (*p == SIGNATURE_float)
	{
		KDEBUG("\t* SIGNATURE_float\n");
		
		value->signature[0] = SIGNATURE_float;
		value->getValue = _getPrimitiveIntValue;
		value->printValue = _printPrimitivieIntValue;
	}
	else if (*p == SIGNATURE_double)
	{
		KDEBUG("\t* SIGNATURE_double\n");
		
		value->signature[0] = SIGNATURE_double;
		value->getValue = _getPrimitiveIntValue;
		value->printValue = _printPrimitivieIntValue;
	}	
	else 
	{
		KDEBUG("\t* SIGNATURE_primitive else\n");
	}
	
	return JVMTI_ERROR_NONE;
}

jvmtiError _parseReferenceSignature(Value* value, char** signature)
{
	char* p = *signature;
	
	if (strncmp(p, SIGNATURE_String, SIGNATURE_String_LENGTH) == 0)
	{
		KDEBUG("\t* SIGNATURE_String\n");
		
		value->getValue = _getReferenceStringValue;
		value->printValue = _printReferenceStringValue;
		
		memcpy(value->signature, SIGNATURE_String, SIGNATURE_String_LENGTH);
		
		*signature += SIGNATURE_String_LENGTH;
	} 
	else if (strncmp(p, SIGNATURE_Integer, SIGNATURE_Integer_LENGTH) == 0)
	{
		KDEBUG("\t* SIGNATURE_Integer\n");
		
		value->getValue = _getReferenceIntegerValue;
		value->printValue = _printReferenceIntegerValue;
		
		memcpy(value->signature, SIGNATURE_Integer, SIGNATURE_Integer_LENGTH);
		
		*signature += SIGNATURE_Integer_LENGTH;
	}
	else if (strncmp(p, SIGNATURE_Float, SIGNATURE_Float_LENGTH) == 0)
	{
		KDEBUG("\t* SIGNATURE_Float\n");
		
		value->getValue = _getReferenceLongValue;
		value->printValue = _printReferenceLongValue;
		
		memcpy(value->signature, SIGNATURE_Float, SIGNATURE_Float_LENGTH);
		
		*signature += SIGNATURE_Float_LENGTH;
	}
	else if (strncmp(p, SIGNATURE_Long, SIGNATURE_Long_LENGTH) == 0)
	{
		KDEBUG("\t* SIGNATURE_Long\n");
		
		value->getValue = _getReferenceLongValue;
		value->printValue = _printReferenceLongValue;
		
		memcpy(value->signature, SIGNATURE_Long, SIGNATURE_Long_LENGTH);
		
		*signature += SIGNATURE_Long_LENGTH;
	}
	else if (strncmp(p, SIGNATURE_Double, SIGNATURE_Double_LENGTH) == 0)
	{
		KDEBUG("\t* SIGNATURE_Double\n");
		
		value->getValue = _getReferenceLongValue;
		value->printValue = _printReferenceLongValue;
		
		memcpy(value->signature, SIGNATURE_Double, SIGNATURE_Double_LENGTH);
		
		*signature += SIGNATURE_Double_LENGTH;
	}
	else 
	{
		KDEBUG("\t* SIGNATURE_reference object\n");
		
		value->getValue = _getReferenceObjectValue;
		value->printValue = _printReferenceObjectValue;
		
		int index = 0;
		char* k = p;
		char objectSignature[SIGNATURE_MAX_LENGTH] = {0, };
		while(*k != SIGNATURE_REFERENCE_POSTFIX)
		{
			objectSignature[index++] = *k;
			*k++;
		}
		KDEBUG("\t\t* object signature(%s)\n", objectSignature);
		
		memcpy(value->signature, objectSignature, index);
		
		*signature += index;
	}
		
	return JVMTI_ERROR_NONE;	
}

jvmtiError getValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, Value* value, jint argumentCount)
{
	if (jvmtiEnv == NULL || jniEnv == NULL || value == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER ;
	}
	
	int index = 0;
	for (index = 0; index < argumentCount; index++) 
	{
		if (value[index].dataType == array) 
		{
			_getReferenceObjectValue(jvmtiEnv, jniEnv, thread, frameDepth, index, &(value[index].data));
		}
		else
		{
			value[index].getValue(jvmtiEnv, jniEnv, thread, frameDepth, index, &(value[index].data));
		}
	}
	
	return JVMTI_ERROR_NONE;	
}

jvmtiError _getReferenceStringValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data)
{	
	jobject jObject;
	
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalObject(jvmtiEnv, thread, frameDepth, slot, &jObject); 
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}
	
	jstring stringValue = (jstring)(*jniEnv)->CallObjectMethod(jniEnv, jObject, methodIdTable.toString);
	*data = (*jniEnv)->GetStringUTFChars(jniEnv, stringValue, frameDepth);
	
	KDEBUG("\t\t* _getReferenceStringValue[%s]\n", *data);
	
	return JVMTI_ERROR_NONE;
}


jvmtiError _getReferenceIntegerValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data)
{
	jobject jObject;
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalObject(jvmtiEnv, thread, frameDepth, slot, &jObject);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}
	
	*data = malloc(sizeof(jint));
	
	*(jint *)(*data) = (*jniEnv)->CallIntMethod(jniEnv, jObject, methodIdTable.intValue);
	
	KDEBUG("\t\t* _getReferenceIntegerValue[%d]\n", *(jint *)*data);
	
	return JVMTI_ERROR_NONE;
}

jvmtiError _getReferenceLongValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data)
{
	jobject jObject;
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalObject(jvmtiEnv, thread, frameDepth, slot, &jObject);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}
	
	*data = malloc(sizeof(jlong));
	
	*(jlong *)(*data) = (*jniEnv)->CallLongMethod(jniEnv, jObject, methodIdTable.longValue);
	
	KDEBUG("\t\t* _getReferenceLongValue[%ld]\n", *(jlong *)(*data));
	
	return JVMTI_ERROR_NONE;
}

jvmtiError _getReferenceObjectValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data)
{
	jobject jObject;
	
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalObject(jvmtiEnv, thread, frameDepth, slot, &jObject);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}
	
	jclass clazz = (*jniEnv)->GetObjectClass(jniEnv, jObject);
	
	jmethodID toStringMethodId = (*jniEnv)->GetMethodID(jniEnv, clazz, "toString", "()Ljava/lang/String;"); 
	jstring stringValue = (jstring)(*jniEnv)->CallObjectMethod(jniEnv, jObject, toStringMethodId);
	
	*data = (*jniEnv)->GetStringUTFChars(jniEnv, stringValue, 0);
	
	KDEBUG("\t\t* _getReferenceObjectValue[%s]\n", *data);
	
	return JVMTI_ERROR_NONE;
}

jvmtiError _getPrimitiveIntValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data)
{
	jint intValue;
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalInt(jvmtiEnv, thread, frameDepth, slot, &intValue);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}		
	
	*data = malloc(sizeof(jint));
	
	*(jint *)(*data)= intValue;
	
	KDEBUG("\t\t* _getPrimitiveIntValue[%d]\n", *(jint *)*data);
	
	return JVMTI_ERROR_NONE;
}


void _printReferenceStringValue(Logger* logger, void* data)
{
	doRecord(logger, "\t\t-argument value(%s)\n", data);
	
	PDEBUG("\t\t\t\t* _getReferenceStringValue[%s]\n", data);
}

void _printReferenceIntegerValue(Logger* logger, void* data)
{
	doRecord(logger, "\t\t-argument value(%d)\n", *(jint *)data);
	
	PDEBUG("\t\t\t\t* _getReferenceIntegerValue[%d]\n", *(jint *)data);
}

void _printReferenceLongValue(Logger* logger, void* data)
{
	doRecord(logger, "\t\t-argument value(%ld)\n", *(jlong *)data);
	
	PDEBUG("\t\t\t\t* _getReferenceLongValue[%ld]\n", *(jlong *)data);
}

void _printReferenceObjectValue(Logger* logger, void* data)
{
	doRecord(logger, "\t\t-argument value(%s)\n", data);
	
	PDEBUG("\t\t\t\t* _getReferenceObjectValue[%s]\n", data);
}

void _printPrimitivieIntValue(Logger* logger, void* data)
{
	doRecord(logger, "\t\t-argument value(%d)\n", *(jint *)data);
	
	PDEBUG("\t\t\t\t* _getPrimitiveIntValue[%d]\n", *(jint *)data);
}