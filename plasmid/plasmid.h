
#ifndef __PLASMID_H__
#define __PLASMID_H__

#include "common.h"
#include "signature.h"

jvmtiError GetFrameCount(jvmtiEnv *jvmtiEnv, jthread thread, jint* frameCount);

jvmtiError GetStackTrace(jvmtiEnv *jvmtiEnv, jthread thread, jvmtiFrameInfo* frameInfo, jint maxFrameCount, jint* frameCount); 

jvmtiError GetMethodInfo(jvmtiEnv *jvmtiEnv, jthread thread, jvmtiFrameInfo* jvmtiFrame, char **methodName, char **signature);
				
jvmtiError GetMethodArgumentCount(jvmtiEnv *jvmtiEnv, jmethodID method, jint* argumentCount);				
				
jvmtiError GetLineNumberTable(jvmtiEnv* jvmtiEnv, jvmtiFrameInfo* jvmtiFrame, jvmtiLineNumberEntry** lineNumberTableEntry, jint** lineNumberTableSize );

	
#endif
