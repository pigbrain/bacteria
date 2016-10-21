

#include "clazz.h"

jvmtiError getClass(jvmtiEnv *jvmtiEnv, jmethodID method, jclass* clazz)
{
	if (jvmtiEnv == NULL)
	{
		return JVMTI_ERROR_NULL_POINTER;
	}
	
	return (*jvmtiEnv)->GetMethodDeclaringClass(jvmtiEnv, method, clazz);
}

bool hasClassLoader(jvmtiEnv *jvmtiEnv, jclass clazz, jmethodID method)
{
	if (jvmtiEnv == NULL)
	{
		return false;
	}
	
	jobject* classLoader;
	(*jvmtiEnv)->GetClassLoader(jvmtiEnv, clazz, &classLoader);
	
	return classLoader != NULL;
}