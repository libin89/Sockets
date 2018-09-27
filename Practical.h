#ifndef _PRACTICAL_H_
#define _PRACTICAL_H_

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG  //debug switch

#define OK 0
#define ERROR -1

#define LOG_EMERG      0    /* system is unusable */  
#define LOG_ALERT      1    /* action must be taken immediately */  
#define LOG_CRIT       2    /* critical conditions */  
#define LOG_ERR        3    /* error conditions */  
#define LOG_WARNING    4    /* warning conditions */  
#define LOG_NOTICE     5    /* normal but significant */  
#define LOG_INFO       6    /* informational */  
#define LOG_DEBUG      7    /* debug-level messages */

#ifdef LOG_FILE
#undef LOG_FILE
#endif
#define LOG_FILE stdout
#define FORMAT_LENGTH 200
#define LOG_LEVEL 7


extern tcp_connection_t *tcpConnection;
char *debugGetTime(void);
void DieWithUserMessage(const char *msg, const char *detail);
void DieWithSystemMessage(const char *msg);

#define printMsg(level, format, outfile, ...)\
do {\
    if(level > LOG_LEVEL) break;\
    fputs(format, outfile);\
}while(0)

#define printInfo(format, args...)\
do {\
    char fmt[FORMAT_LENGTH] = {0};\
    snprintf(fmt, FORMAT_LENGTH, "%s %s:%d(%s) - %s",debugGetTime(),\
            __FILE__, __LINE__, __FUNCTION__, format);\
    printMsg(LOG_INFO, fmt, LOG_FILE, ## args);\
}while(0)

#define printErr(format, args...)\
do {\
    char fmt[FORMAT_LENGTH] = {0};\
    snprintf(fmt, FORMAT_LENGTH, "%s %s:%d(%s) - %s",debugGetTime(),\
            __FILE__, __LINE__, __FUNCTION__, format);\
    printMsg(LOG_ERR, fmt, LOG_FILE, ## args);\
}while(0)

#ifdef DEBUG
#define printDebug(format, args...) \
do {\
    char fmt[FORMAT_LENGTH] = {0};\
    snprintf(fmt, FORMAT_LENGTH, "%s %s:%d(%s) - %s",debugGetTime(),\
            __FILE__, __LINE__, __FUNCTION__, format);\
    printMsg(LOG_DEBUG, fmt, LOG_FILE, ## args);\
}while(0)
#else
#define printDebug(format, args...)
#endif



#endif