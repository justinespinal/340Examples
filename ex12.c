#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[]){
    int fd[2];
    pipe(fd);

    if(fork() == 0){ // C2 (/bin/wc)
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);

        execl("/bin/wc", "wc", NULL);
    }
    close(fd[0]);
    int pid;
    if((pid = fork()) == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        execl("/bin/cat", "cat", "file.txt", NULL);
    }
    close(fd[1]);
    waitpid(pid, NULL, 0);
    printf("Done!\n");


    return 0;
}