

#ifndef __SIGNATURE_H__
#define __SIGNATURE_H__

#include "common.h"
#include "logger.h"
 
#define SIGNATURE_MAX_LENGTH        100

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
	_undefined, // 0
	_String, // 1
	_Integer, // 2
	_Long, // 3
	_Float, // 4
	_Double, // 5
	_boolean, // 6
	_byte, // 7
	_char, // 8
	_short, // 9
	_int, // 10
	_long, // 11
	_float, // 12
	_double, // 13
	_void, // 14
	_array // 15
} SignatureCode;

typedef struct 
{
	jmethodID toString;	// String class
	jmethodID intValue;	// Integer class
	jmethodID longValue; // Long class
	jmethodID floatValue; // Float class
	jmethodID doubleValue; // Double class
} MethodIdTable;

typedef struct 
{
	SignatureCode signatureCode;
	void* value;
} SignatureValue;


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

jvmtiError parseMethodSignature(char* signature, jint argumentCount, SignatureCode** signatureCodes, jint* signatureCodeCount);

bool isObject(char* signature);

SignatureCode getPrimitiveSignatureCode(char* signature);

SignatureCode getReferenceSignatureCode(char** signature);

jvmtiError getValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureCode signatureCode, SignatureValue** signatureValue);

jvmtiError getReferenceStringValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue);

jvmtiError getReferenceIntegerValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue);

jvmtiError getReferenceLongValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue);

jvmtiError getPrimitiveIntValue(jvmtiEnv *jvmtiEnv, JNIEnv* jniEnv, jthread thread, jint slot, SignatureValue** signatureValue);

void printSignatureValue(SignatureValue* signatureValue, Logger* logger);
 
#endif





