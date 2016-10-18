

#include "clazz.h"

bool hasClassLoader(jvmtiEnv *jvmtiEnv, jmethodID method)
{
	if (jvmtiEnv == NULL)
	{
		return false;
	}
	
	jclass declaringClass;
	if ((*jvmtiEnv)->GetMethodDeclaringClass(jvmtiEnv, method, &declaringClass) != JVMTI_ERROR_NONE) 
	{
		return false;
	}
	
	jobject* classLoader;
	(*jvmtiEnv)->GetClassLoader(jvmtiEnv, declaringClass, &classLoader);
	
	return classLoader != NULL;
}