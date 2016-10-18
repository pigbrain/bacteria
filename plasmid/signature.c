
#include "signature.h"

MethodIdTable methodIdTable;

jvmtiError initMethodIdTable(JNIEnv* jniEnv)
{
	methodIdTable.toString  = getToStringMethodIdOfString(jniEnv);
	methodIdTable.intValue  = getIntValueMethodIdOfInteger(jniEnv);
	methodIdTable.longValue  = getLongValueMethodIdOfLong(jniEnv);
	methodIdTable.floatValue  = getFloatValueMethodIdOfFloat(jniEnv);
	methodIdTable.doubleValue  = getDoubleValueMethodIdOfDouble(jniEnv);
}


jvmtiError findMethodId(JNIEnv* jniEnv, jchar* className, jchar* methodSignature, jchar* methodName, jmethodID* methodId)
{
	if (jniEnv == NULL || className == NULL || methodSignature == NULL || methodName == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER ;
	}
	
	jclass clazz = (*jniEnv)->FindClass(jniEnv, className);
	
	*methodId = (*jniEnv)->GetMethodID(jniEnv, clazz, methodName, methodSignature);
	
	return JVMTI_ERROR_NONE;
}

jvmtiError getObjectSignature(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jobject object, char** signature) 
{
	jclass clazz = (*jniEnv)->GetObjectClass(jniEnv, object);
    
	return (*jvmtiEnv)->GetClassSignature(jvmtiEnv, clazz, signature, NULL);
}

jvmtiError getClassSignature(jvmtiEnv *jvmtiEnv, jmethodID method, char** signature)
{
	jvmtiError error;
	jclass declaringClass;
	
	error = (*jvmtiEnv)->GetMethodDeclaringClass(jvmtiEnv, method, &declaringClass);
	if (error != JVMTI_ERROR_NONE) 
	{
		return error;
	}
	
	return (*jvmtiEnv)->GetClassSignature(jvmtiEnv, declaringClass, signature, NULL);
}

jmethodID getToStringMethodIdOfString(JNIEnv* jniEnv)
{
	if (jniEnv == NULL) 
	{
		return 0;
	}
	
	jchar methodSignature[SIGNATURE_MAX_LENGTH] = {0, };
    snprintf(methodSignature, SIGNATURE_MAX_LENGTH, "()%c%s%c", 
			SIGNATURE_REFERENCE_PREFIX,
			SIGNATURE_String,
			SIGNATURE_REFERENCE_POSTFIX);

	jmethodID methodId;
	jvmtiError jvmtiErr = findMethodId(jniEnv, SIGNATURE_String, methodSignature, "toString", &methodId);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return 0;
	}
	
	return methodId;
}

jmethodID getIntValueMethodIdOfInteger(JNIEnv* jniEnv)
{
	if (jniEnv == NULL) 
	{
		return 0;
	}
	
	jchar methodSignature[SIGNATURE_MAX_LENGTH] = {0, };
    snprintf(methodSignature, SIGNATURE_MAX_LENGTH, "()%c", SIGNATURE_int);
			
	jmethodID methodId;
	jvmtiError jvmtiErr = findMethodId(jniEnv, SIGNATURE_Integer, methodSignature, "intValue", &methodId);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return 0;
	}
	
	return methodId;
}

jmethodID getLongValueMethodIdOfLong(JNIEnv* jniEnv)
{
	if (jniEnv == NULL) 
	{
		return 0;
	}
	
	jchar methodSignature[SIGNATURE_MAX_LENGTH] = {0, };
    snprintf(methodSignature, SIGNATURE_MAX_LENGTH, "()%c", SIGNATURE_long);
	
	jmethodID methodId;
	jvmtiError jvmtiErr = findMethodId(jniEnv, SIGNATURE_Long, methodSignature, "longValue", &methodId);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return 0;
	}
	
	return methodId;
}

jmethodID getFloatValueMethodIdOfFloat(JNIEnv* jniEnv)
{
	if (jniEnv == NULL) 
	{
		return 0;
	}
		
	jchar methodSignature[SIGNATURE_MAX_LENGTH] = {0, };
    snprintf(methodSignature, SIGNATURE_MAX_LENGTH, "()%c", SIGNATURE_float);
	
	jmethodID methodId;
	jvmtiError jvmtiErr = findMethodId(jniEnv, SIGNATURE_Float, methodSignature, "floatValue", &methodId);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return 0;
	}
	
	return methodId;
}

jmethodID getDoubleValueMethodIdOfDouble(JNIEnv* jniEnv)
{
	if (jniEnv == NULL) 
	{
		return 0;
	}
	
	jchar methodSignature[SIGNATURE_MAX_LENGTH] = {0, };
    snprintf(methodSignature, SIGNATURE_MAX_LENGTH, "()%c", SIGNATURE_double);
	
	jmethodID methodId;
	jvmtiError jvmtiErr = findMethodId(jniEnv, SIGNATURE_Double, methodSignature, "doubleValue", &methodId);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return 0;
	}
	
	return methodId;
}

jvmtiError parseMethodSignature(char* signature, jint argumentCount, SignatureCode** signatureCodes, jint* signatureCodeCount) 
{	
	*signatureCodes = malloc(argumentCount * sizeof(SignatureCode));

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
		
		if (isObject(p))
		{
			p++; // prefix : L
			(*signatureCodes)[index] = getReferenceSignatureCode(&p);
			p++; // postfix : ;
		}
		else 
		{
			(*signatureCodes)[index] = getPrimitiveSignatureCode(p);
			
			p++;
		}
		
		index++;
	}
	
	*signatureCodeCount = index;
	
	return JVMTI_ERROR_NONE;
}

bool isObject(char* signature)
{
	return (*signature == SIGNATURE_REFERENCE_PREFIX);
}

SignatureCode getPrimitiveSignatureCode(char* signature)
{
	if (*signature == SIGNATURE_int)
	{
		return _int;
	}
	
	if (*signature == SIGNATURE_long)
	{
		return _long;
	}
	
	if (*signature == SIGNATURE_float)
	{
		return _float;
	}
	
	if (*signature == SIGNATURE_double)
	{
		return _double;
	}	
	
	return _undefined;
}

SignatureCode getReferenceSignatureCode(char** signature) 
{
	if (strncmp(*signature, SIGNATURE_String, SIGNATURE_String_LENGTH) == 0)
	{
		*signature += SIGNATURE_String_LENGTH;
		
		return _String;
	}
	
	if (strncmp(*signature, SIGNATURE_Integer, SIGNATURE_Integer_LENGTH) == 0)
	{
		*signature += SIGNATURE_Integer_LENGTH;
		
		return _Integer;
	}
	
	if (strncmp(*signature, SIGNATURE_Float, SIGNATURE_Float_LENGTH) == 0)
	{
		*signature += SIGNATURE_Float_LENGTH;
		
		return _Float;
	}
	
	if (strncmp(*signature, SIGNATURE_Long, SIGNATURE_Long_LENGTH) == 0)
	{
		*signature += SIGNATURE_Long_LENGTH;
		
		return _Long;
	}
	
	if (strncmp(*signature, SIGNATURE_Double, SIGNATURE_Double_LENGTH) == 0)
	{
		*signature += SIGNATURE_Double_LENGTH;
		
		return _Double;
	}
	
	return _undefined;
}

jvmtiError getValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureCode signatureCode, SignatureValue** signatureValue)
{
	if (jvmtiEnv == NULL || jniEnv == NULL) 
	{
		return JVMTI_ERROR_NULL_POINTER ;
	}
	
	*signatureValue = malloc(sizeof(SignatureValue));
	(*signatureValue)->signatureCode = signatureCode;
	
	switch (signatureCode)
	{
		case _String :
			return getReferenceStringValue(jvmtiEnv, jniEnv, thread, slot, signatureValue);
		case _Integer :
			return getReferenceIntegerValue(jvmtiEnv, jniEnv, thread, slot, signatureValue);
		case _Long :
			return getReferenceLongValue(jvmtiEnv, jniEnv, thread, slot, signatureValue);
		case _Float :
			break;
		case _Double :
			break;
		case _boolean :
			break;
		case _byte :
			break;
		case _char :
			break;
		case _short :
			break;
		case _int :
			return getPrimitiveIntValue(jvmtiEnv, jniEnv, thread, slot, signatureValue);
		case _long :
			break;
		case _float :
			break;
		case _double :
			break;
		case _void :
			break;
		case _array :
			break;
		default :
			break;
	}
	
	return JVMTI_ERROR_NONE;
}

jvmtiError getReferenceStringValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue) 
{
	jobject jObject;
	
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalObject(jvmtiEnv, thread, 0, slot, &jObject); 
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}
			
	jstring stringValue = (jstring)(*jniEnv)->CallObjectMethod(jniEnv, jObject, methodIdTable.toString);
	
	(*signatureValue)->value = (*jniEnv)->GetStringUTFChars(jniEnv, stringValue, 0);
	
	return JVMTI_ERROR_NONE;
}

jvmtiError getReferenceIntegerValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue)
{
	jobject jObject;
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalObject(jvmtiEnv, thread, 0, slot, &jObject);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}
	
	(*signatureValue)->value = malloc(sizeof(jint));
	
	*(jint *)((*signatureValue)->value) = (*jniEnv)->CallIntMethod(jniEnv, jObject, methodIdTable.intValue);
	
	return JVMTI_ERROR_NONE;
}

jvmtiError getReferenceLongValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue)
{
	jobject jObject;
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalObject(jvmtiEnv, thread, 0, slot, &jObject);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}
	
	(*signatureValue)->value = malloc(sizeof(jlong));
	
	*(jlong *)((*signatureValue)->value) = (*jniEnv)->CallLongMethod(jniEnv, jObject, methodIdTable.longValue);
	
	return JVMTI_ERROR_NONE;
}

jvmtiError getPrimitiveIntValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue) 
{
	jint intValue;
	jvmtiError jvmtiErr = (*jvmtiEnv)->GetLocalInt(jvmtiEnv, thread, 0, slot, &intValue);
	if (jvmtiErr != JVMTI_ERROR_NONE)
	{
		return jvmtiErr;
	}		
	
	(*signatureValue)->value = malloc(sizeof(jint));
	
	*((jint *)((*signatureValue)->value))= intValue;
	
	return JVMTI_ERROR_NONE;
}

void printSignatureValue(SignatureValue* signatureValue)
{
	if (signatureValue == NULL) 
	{
		return;
	}
	
	switch (signatureValue->signatureCode)
	{
		case _String :
			PDEBUG("\t\t\t\t\t get reference String value(%s)\n", (char *)signatureValue->value);
			break;
		case _Integer :
			PDEBUG("\t\t\t\t\t get reference Integer value(%d)\n", *((jint *)signatureValue->value));
			break;
		case _Long :
			PDEBUG("\t\t\t\t\t get reference Long value(%ld)\n", *((jlong *)signatureValue->value));
			break;
		case _Float :
			break;
		case _Double :
			break;
		case _boolean :
			break;
		case _byte :
			break;
		case _char :
			break;
		case _short :
			break;
		case _int :
			PDEBUG("\t\t\t\t\t get primitive int value(%d)\n", *((jint *)signatureValue->value));
			break;
		case _long :
			break;
		case _float :
			break;
		case _double :
			break;
		case _void :
			break;
		case _array :
			break;
		default :
			break;
	}
}