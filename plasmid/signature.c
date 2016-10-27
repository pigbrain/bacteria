
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
