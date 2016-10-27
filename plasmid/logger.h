
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "common.h"


#define LOG_FILE_NAME_MAX_LEN    512
#define LOG_FILE_PATH_MAX_LEN    1024
#define LOG_FILE_PATH_MAX_LEN    1024
#define LOG_MAX_LEN              4096

typedef struct 
{
	bool active;
	FILE *file;
	int offset;
	char buffer[LOG_MAX_LEN];
} Logger;
 
extern char* logLocation;

void startRecord(Logger** logger, char* exceptionName);

void doRecord(Logger* logger, char *format, ...);

void endRecord(Logger** logger);

void normalizeName(char* input, char* output);

#endif

