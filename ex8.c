#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[]){
    
    int fd[2];
    pipe(fd);

    if(fork() == 0){
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);

        execl("/bin/wc", "wc", NULL);
    }

    dup2(fd[1], 1);
    close(fd[1]);
    close(fd[0]);
    int file = open("file.txt", O_RDONLY);
    char ch;
    while(read(file, &ch, 1) > 0){
        write(1, &ch, 1);
    }
    close(file);
    return 0;
}