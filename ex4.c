#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
    int fd[2];
    pipe(fd);

    if(fork() == 0){ // C1
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);

        printf("Hello\n");
        exit(0);
    }
    close(fd[1]);
    int pid;
    if((pid = fork()) == 0){ // C2
        dup2(fd[0], 0);
        close(fd[0]);
        char message[6];
        read(0, message, 6);
        printf("----%s", message);
        exit(0);
    }
    close(fd[0]);
    waitpid(pid, NULL, 0);
    printf("Parent done...\n");


    return 0;
}