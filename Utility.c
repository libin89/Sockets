#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

void DieWithUserMessage(const char *msg, const char *detail)
{
	fputs(msg, stderr);
	fputs(": ", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	deinitializeTCPData();
	exit(1);
}

void DieWithSystemMessage(const char *msg)
{
	fputs(msg, stderr);
	deinitializeTCPData();
	exit(1);
}

char *debugGetTime(void)
{
#define TIMESTR_SIZE 50
	char timestr[TIMESTR_SIZE] = {0};
	time_t timep = 0;
    struct tm *p = NULL;
	struct timeval tv = {0};
    struct timezone tz = {0};

    gettimeofday(&tv, &tz);
    time(&timep);
    p = localtime(&timep);
    snprintf(timestr, TIMESTR_SIZE, "curday = %d-%02d-%02d %02d:%02d:%02d.%ld",\
		(1900+p->tm_year), (1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec,\
		tv.tv_usec);

	return timestr;
}
