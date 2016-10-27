
#ifndef __PLASMID_H__
#define __PLASMID_H__

#include "common.h"
#include "signature.h"
#include "clazz.h"
#include "logger.h"
#include "value.h"

jvmtiError getFrameCount(jvmtiEnv *jvmtiEnv, jthread thread, jint* frameCount);

jvmtiError getStackTrace(jvmtiEnv *jvmtiEnv, jthread thread, jvmtiFrameInfo* frameInfo, jint maxFrameCount, jint* frameCount); 

jvmtiError getMethodInfo(jvmtiEnv *jvmtiEnv, jthread thread, jmethodID method, char **methodName, char **signature);
				
jvmtiError getMethodArgumentCount(jvmtiEnv *jvmtiEnv, jmethodID method, jint* argumentCount);				
				
jvmtiError getLineNumberTable(jvmtiEnv* jvmtiEnv, jmethodID method, jvmtiLineNumberEntry** lineNumberTableEntry, jint** lineNumberTableSize );

jvmtiError getLineNumber(jvmtiLineNumberEntry* lineNumberTableEntry, jint lineNumberTableSize, jvmtiFrameInfo* jvmtiFrame, jint* codeLineNumber);
		
	
#endif
