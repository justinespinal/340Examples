#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[]){
    int fd[2];
    pipe(fd);

    if(fork() == 0){ // C2 (Write)
        close(fd[1]);
        int file = open("file2.txt", O_WRONLY);
        char message[10];
        read(fd[0], message, 10);
        close(fd[0]);
        write(file, message, 10);
        exit(0);
    }
    close(fd[0]);
    int pid;
    if((pid = fork()) == 0){ // C1
        dup2(fd[1], 1);
        close(fd[1]);
        execl("/bin/cat", "cat", "file.txt", NULL);
    }
    waitpid(pid, NULL, 0);
    printf("Done!\n");
    return 0;
}