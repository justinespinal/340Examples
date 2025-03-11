#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
    int fd2[2];
    pipe(fd2);

    int pid2;
    if((pid2 = fork()) == 0){ // C2
        close(fd2[1]);
        char message[6];
        read(fd2[0], message, 6);
        write(1, message, 6);
        exit(0);
    }
    close(fd2[0]);
    int fd1[2];
    pipe(fd1);
    int pid1;
    if((pid1 = fork()) == 0){ //C1
        char message[6];
        read(fd1[0], message, 6);
        write(fd2[1], message, 6);
        exit(0);
    }
    close(fd1[0]);
    write(fd1[1], "Hello\n", 6);
    waitpid(pid1, NULL, 0);
    printf("Done\n");
    return 0;
}

