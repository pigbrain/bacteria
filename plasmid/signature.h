

#ifndef __SIGNATURE_H__
#define __SIGNATURE_H__

#include "common.h" 
#include "logger.h"
 


typedef struct 
{
	jmethodID toString;	// String class
	jmethodID intValue;	// Integer class
	jmethodID longValue; // Long class
	jmethodID floatValue; // Float class
	jmethodID doubleValue; // Double class
} MethodIdTable;

extern MethodIdTable methodIdTable;

jvmtiError initMethodIdTable(JNIEnv* jniEnv);

jvmtiError findMethodId(JNIEnv* jniEnv, jchar* className, jchar* methodSignature, jchar* methodName, jmethodID* methodId);

jvmtiError getObjectSignature(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jobject object, char** signature);

jvmtiError getClassSignature(jvmtiEnv *jvmtiEnv, jmethodID method, char** signature);

jmethodID getToStringMethodIdOfString(JNIEnv* jniEnv);

jmethodID getIntValueMethodIdOfInteger(JNIEnv* jniEnv);

jmethodID getLongValueMethodIdOfLong(JNIEnv* jniEnv);

jmethodID getFloatValueMethodIdOfFloat(JNIEnv* jniEnv);

jmethodID getDoubleValueMethodIdOfDouble(JNIEnv* jniEnv);

#endif






