#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
/* /bin/ls | /bin/wc */
int main(int argc, char *argv[]){
    int fd[2];
    pipe(fd);

    if(fork()==0){ // C1 (/bin/ls)
        dup2(fd[1], 1); // whats output to the screen is input to the pipe
        close(fd[0]);
        close(fd[1]);

        execl("/bin/ls", "ls", NULL);
    }
    close(fd[1]);
    int pid;
    if((pid = fork()) == 0){ // C2 ("/bin/wc")
        dup2(fd[0], 0);
        close(fd[0]);

        execl("/bin/wc", "wc", NULL);
    }
    close(fd[0]);
    waitpid(pid, NULL, 0);
    printf("done!\n");


    return 0;
}