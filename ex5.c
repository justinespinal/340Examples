#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
    int fd[2];
    pipe(fd);

    int pid;
    if((pid = fork())==0){
        close(fd[1]);
        char message[6];
        read(fd[0], message, 6);
        close(fd[0]);
        printf("%s", message);
        exit(0);
    }
    close(fd[0]);
    write(fd[1], "Hello\n", 6);
    close(fd[1]);
    waitpid(pid, NULL, 0);
    printf("Done\n");
    return 0;
}