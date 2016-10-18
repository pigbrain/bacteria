
#include "logger.h"
#include "signature.h"

char* logLocation = ".";

void startRecord(Logger** logger, char* exceptionName)
{
	*logger = malloc(sizeof(Logger));
	
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
  
	char logFileName[LOG_FILE_NAME_MAX_LEN] = {0, };
	normalizeName(exceptionName, logFileName);
	
	char logFilePath[LOG_FILE_PATH_MAX_LEN] = {0, };
	snprintf(logFilePath, LOG_FILE_PATH_MAX_LEN, "%s/%04d%02d%02d%02d%02d%02d_%s", logLocation, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, logFileName);
	
	PDEBUG("# start record at location(%s)\n", logFilePath);
	
	(*logger)->file = fopen(logFilePath,"w");
	if ((*logger)->file != NULL) 
	{
		(*logger)->active = true;
		doRecord(*logger, "%s\n", logFileName);
	}
	else 
	{
		(*logger)->active = false;
	}
}

void doRecord(Logger* logger, char *format, ...)
{
	if (logger->file == NULL || !(logger->active))
	{
		return;
	}
	
	char log[LOG_MAX_LEN] = {0, };
	
	va_list ap;
	va_start(ap, format);
	vsnprintf(log, sizeof(log), format, ap);
	va_end(ap);

	fprintf(logger->file,"%s", log);
    fflush(logger->file);
}

void endRecord(Logger** logger)
{
	if ((*logger)->file != NULL)
	{
		fclose((*logger)->file);
		SAFE_FREE(*logger);
	}
}

void normalizeName(char* input, char* output)
{
	char* p = input;
	char* o = output;
	while (*p)
	{
		if (*p == '/')
		{
			*o = '.';			
		}
		else if(*p == SIGNATURE_REFERENCE_POSTFIX)
		{
			// nothing
		}
		else
		{
			*o = *p;
		}
		
		o++;
		p++;
	}
	
	PDEBUG("?%s\n", output);
}