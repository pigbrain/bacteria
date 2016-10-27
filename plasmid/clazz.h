
#ifndef __CLAZZ_H__
#define __CLAZZ_H__

#include "common.h"


jvmtiError getClass(jvmtiEnv *jvmtiEnv, jmethodID method, jclass* clazz);

bool hasClassLoader(jvmtiEnv *jvmtiEnv, jclass clazz, jmethodID method);

#endif


