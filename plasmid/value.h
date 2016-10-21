
#ifndef __VALUE_H__
#define __VALUE_H__


#include "common.h"
#include "signature.h"
#include "logger.h"

typedef jvmtiError (*getValuePtr)(jvmtiEnv*, JNIEnv*, jthread, jint frameDepth, jint/*slot*/, void** /*data*/);
typedef void (*printValuePtr)(Logger*, void* /*data*/);

typedef struct 
{
	DataType dataType;
	char signature[SIGNATURE_MAX_LENGTH];
	//char fieldName[FIELDNAME_MAX_LENGTH];
	void* data;
	getValuePtr getValue;
	printValuePtr printValue;
} Value;

jvmtiError initValues(Value** values, jint argumentCount, char* signature);

jvmtiError destroyValues(Value* values, jint argumentCount);

bool _isReference(char** signature);

bool _isArray(char** signature);

jvmtiError _parseSignature(Value* value, char** signature);

jvmtiError _parsePrimitiveSignature(Value* value, char** signature);

jvmtiError _parseReferenceSignature(Value* value, char** signature);

jvmtiError getValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, Value* value, jint argumentCount);

jvmtiError _getReferenceStringValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data);

jvmtiError _getReferenceIntegerValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data);

jvmtiError _getReferenceLongValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data);

jvmtiError _getReferenceObjectValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data);

jvmtiError _getPrimitiveIntValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint frameDepth, jint slot, void** data);

void _printReferenceStringValue(Logger* logger, void* data);

void _printReferenceIntegerValue(Logger* logger, void* data);

void _printReferenceLongValue(Logger* logger, void* data);

void _printReferenceObjectValue(Logger* logger, void* data);

void _printPrimitivieIntValue(Logger* logger, void* data);

#endif 

