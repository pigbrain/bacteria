


#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "jni.h"
#include "jvmti.h" 


#define STACK_FRAME_MAX_COUNT 20

#define _PDEBUG
//#define _KDEBUG

#ifdef _PDEBUG
#define PDEBUG(fmt, args...) printf( fmt, ## args )
#else
#define PDEBUG(fmt, args...) 
#endif

#ifdef _KDEBUG
#define KDEBUG(fmt, args...) printf( fmt, ## args )
#else
#define KDEBUG(fmt, args...) 
#endif


#ifndef SAFE_FREE
#define SAFE_FREE(p) \
    if (p) \
    { free (p); (p) = NULL; }
#endif


#define FIELDNAME_MAX_LENGTH        512 

#define SIGNATURE_MAX_LENGTH        512

#define SIGNATURE_START             '('
#define SIGNATURE_END               ')'

#define SIGNATURE_REFERENCE_PREFIX  'L'
#define SIGNATURE_REFERENCE_POSTFIX ';'

#define SIGNATURE_String            "java/lang/String"
#define SIGNATURE_Integer           "java/lang/Integer"
#define SIGNATURE_Long              "java/lang/Long"
#define SIGNATURE_Float             "java/lang/Float"
#define SIGNATURE_Double            "java/lang/Double"

#define SIGNATURE_String_LENGTH     strlen(SIGNATURE_String)
#define SIGNATURE_Integer_LENGTH    strlen(SIGNATURE_Integer)
#define SIGNATURE_Long_LENGTH       strlen(SIGNATURE_Long)
#define SIGNATURE_Float_LENGTH      strlen(SIGNATURE_Float)
#define SIGNATURE_Double_LENGTH     strlen(SIGNATURE_Double)

#define SIGNATURE_boolean           'Z'
#define SIGNATURE_byte              'B'
#define SIGNATURE_char              'C'
#define SIGNATURE_short             'S'
#define SIGNATURE_int               'I'
#define SIGNATURE_long              'J'
#define SIGNATURE_float             'F'
#define SIGNATURE_double            'D'

#define SIGNATURE_void              'V'
#define SIGNATURE_array	            '['


typedef enum 
{
	false, 
	true
} bool;

typedef enum 
{
	undefined,
	primitive,
	reference,
	array
} DataType;

static jvmtiEnv *jvmti;

#endif


