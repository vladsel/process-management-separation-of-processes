#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>

#define SIZE 256

void lastError(const char *lastErr) {
    printf("\ndev: error %s\n", lastErr);
    printf("strerror: %s, errno = %d\n\n", strerror(errno), errno);
    exit(1);
}

int main(int argc, char **argv) {
    
	int logFileParent = open("log_parent.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (logFileParent == -1) {
        close(logFileParent);
        lastError("opening Parent log file");
    }
    
    if (write(logFileParent, "\n[Parent]: new Program started\n",
            strlen("\n[Parent]: new Program started\n")) != strlen("\n[Parent]: new Program started\n")) {
        close(logFileParent);
        lastError("write to Parent log file");
    }
    
    printf("fork in progress\n\n");
    pid_t PID = fork();
    
    if (PID == 0) {
        if (setsid() == -1)
            lastError("[Child]: setsid() == -1");
        
        char *dirPath = malloc(PATH_MAX);
        getcwd(dirPath, PATH_MAX);
        if (chdir("/") == -1)
            lastError("[Child]: change directory");
        
        if (write(logFileParent, "\n[Child]: close Parent's log file\n\n",
                strlen("\n[Child]: close Parent's log file\n\n")) != strlen("\n[Child]: close Parent's log file\n\n")) {
            close(logFileParent);
            lastError("[Child]: write to Parent log file");
        }
        if (close(logFileParent) == -1)
            lastError("[Child]: close Parent's log file");
        
        int devNull = open("/dev/null", O_WRONLY);
        if (devNull == -1) {
            close(devNull);
            lastError("[Child]: opening /dev/null");
        }
        int in = dup2(devNull, 0);
        int out = dup2(devNull, 1);
        int err = dup2(devNull, 2);
        if ((in == -1) || (out == -1) || (err == -1))
            lastError("[Child]: opening standard streams");
        
        int logFileChild = open(strcat(dirPath, "/log_child.txt"), O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (logFileChild == -1) {
            close(logFileChild);
            lastError("[Child]: opening Child log file");
        }
        if (write(logFileChild, "\n\n[Child]: new demon process param:\n",
                strlen("\n\n[Child]: new demon process param:\n")) != strlen("\n\n[Child]: new demon process param:\n")) {
            close(logFileChild);
            lastError("[Child]: write to Child log file");
        }
        
        size_t checkVal = 0;
        char buf[SIZE] = { };
        while(1) {
            if (getsid(getpid()) == -1)
                lastError("[Child]: getsid(getpid()) == -1");
            
            checkVal = snprintf(buf, SIZE, "\n[Child]: getpid = %d, getgid = %d, getsid = %d\n",  getpid(), getgid(), getsid(getpid()));
            if (checkVal < 0 && checkVal >= SIZE)
                lastError("[Child]: check value");
            
            if (write(logFileChild, buf, strlen(buf)) != strlen(buf)) {
                close(logFileChild);
                lastError("[Child]: write to Child log file");
			}

			sleep(3);
        }
    }
    else if (PID > 0) {
        if (write(logFileParent, "\n[Parent]: new Child process created\n",
                strlen("\n[Parent]: new Child process created\n")) != strlen("\n[Parent]: new Child process created\n")) {
            close(logFileParent);
            lastError("[Parent]: write to Parent log file");
        }
    }
    else {
        close(logFileParent);
        lastError("fork() PID == -1");
    }
    
    return 0;
}
