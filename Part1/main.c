#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void lastError(const char *lastErr) {
    printf("\ndev: error %s\n", lastErr);
    printf("strerror: %s, errno = %d\n\n", strerror(errno), errno);
    exit(1);
}

void printProcInfo(const char *curProc) {
    if (getsid(getpid()) == -1)
        lastError(strcat("getsid(getpid()) == -1: ", curProc));
    printf("Current process is: [%s]\n", curProc);
    printf("getpid = %d, getgid = %d, getsid = %d\n\n",  getpid(), getgid(), getsid(getpid()));
}

int main(int argc, char **argv) {
    printf("\nProgram started\n\n");
    
    printProcInfo("Parent");
    
    printf("fork in progress\n\n");
    pid_t PID = fork();
    
    if (PID == 0)
        for (size_t i = 0; i < 5; i++) {
            printProcInfo("Child");
            sleep(1);
        }
    else if(PID > 0) {
        for (size_t i = 0; i < 3; i++) {
            printProcInfo("Parent");
            sleep(1);
        }
        printf("[Parent] process is waiting Child process\n\n");
        if (wait(NULL) == -1)
            lastError("[Parent]: wait(NULL) == -1");
    }
    else 
        lastError("fork() PID == -1");
    
    printf("Program finished\n");
    if (PID == 0)
        printProcInfo("Child");
    else
        printProcInfo("Parent");
    
    return 0;
}
