#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define PARAMETERS_LENGTH 100
char* getshortname(char* strname)
{
	char *pTmp;
	if (strname)
	{
		pTmp = strrchr(strname, '/');
		return pTmp != NULL ? pTmp + 1 : strname;
	}
	else
		return 0;
}


int main(int argc, char** argv)
{
    int i;
    char *child_argv[PARAMETERS_LENGTH] = {0};
    pid_t pid;
    if (argc < 2) {
        fprintf(stderr, "Usage:%s <exe_path> <args...>", argv[0]);
        return -1;
    }
    
    for (i = 1; i < argc; ++i) {
        child_argv[i-1] = (char *)malloc(strlen(argv[i])+1);
        strncpy(child_argv[i-1], argv[i], strlen(argv[i]));
        child_argv[i-1][strlen(argv[i])] = '\0';
    }
    child_argv[i] = NULL;
 
	int status = 0;
	char killcmd[PARAMETERS_LENGTH] = "killall -9 ";
    strcat(killcmd, getshortname(argv[1]));
 
	while (1)
	{
		system(killcmd);

		pid_t pid = fork();
		if (pid < 0) //error
		{
			printErr("[Deamon] fork err=%d:%s\n", errno, strerror(errno));
			sleep(5);
			continue;
		}
		else if (pid == 0) //Child process
		{
			printInfo("[Deamon] fork %s ok!\n", child_argv[0]);
			execvp(child_argv[0], (char **)child_argv);
			exit( -1);
		}
        else //Parent process
		{
		    printInfo("[Deamon] child pid=%d ok!\n", pid);
            pid_t exitPid = wait(&status); //blocking wait
            printInfo("[Deamon] pid=%d exitPid=%d!\n", pid, exitPid);
            sleep(5);
		}		
	}	

	return 0;
}

