


#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jni.h"
#include "jvmti.h" 


#define STACK_FRAME_MAX_COUNT 10

#define _PDEBUG

#ifdef _PDEBUG
#define PDEBUG(fmt, args...) printf( fmt, ## args )
#else
#define PDEBUG(fmt, args...) 
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p) \
    if (p) \
    { free (p); (p) = NULL; }
#endif

typedef enum 
{
	false, 
	true
} bool;


static jvmtiEnv *jvmti;

#endif


