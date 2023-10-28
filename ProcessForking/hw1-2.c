#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//fork 出 child
void forkprocess(char *command) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        execlp(command, command, NULL);
        perror("execute failed");
        exit(1);
    } else {
        wait(NULL);
    }
}

//讀取 user input
void Shell_commmand() {
    char cmdInput[1024];

    while (1) {
        printf("osh> ");
        fflush(stdout);
        //read user input(cmdInput) from stdin
        if (fgets(cmdInput, sizeof(cmdInput), stdin) != NULL) {
            
            int len = strlen(cmdInput);
            if (len > 0 && cmdInput[len - 1] == '\n') {
                cmdInput[len - 1] = '\0';//command is over
            }

            //input exit break while loop
            if (strcmp(cmdInput, "exit") == 0) {
                break;
            }
            forkprocess(cmdInput);
        }
    }
}

int main() {
    Shell_commmand();
    printf("\n");
    return 0;
}
