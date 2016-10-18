
#ifndef __PLASMID_H__
#define __PLASMID_H__

#include "common.h"
#include "signature.h"

jvmtiError getFrameCount(jvmtiEnv *jvmtiEnv, jthread thread, jint* frameCount);

jvmtiError getStackTrace(jvmtiEnv *jvmtiEnv, jthread thread, jvmtiFrameInfo* frameInfo, jint maxFrameCount, jint* frameCount); 

jvmtiError getMethodInfo(jvmtiEnv *jvmtiEnv, jthread thread, jvmtiFrameInfo* jvmtiFrame, char **methodName, char **signature);
				
jvmtiError getMethodArgumentCount(jvmtiEnv *jvmtiEnv, jmethodID method, jint* argumentCount);				
				
jvmtiError getLineNumberTable(jvmtiEnv* jvmtiEnv, jvmtiFrameInfo* jvmtiFrame, jvmtiLineNumberEntry** lineNumberTableEntry, jint** lineNumberTableSize );

	
#endif
