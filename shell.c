#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

void tokenize(char input[], char *args[]){
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < 9) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void addParams( char *args[], char *params[], int start, int end){
    for( int i = start; i <= end; i++ ){
        params[i] = args[i];
    }
}

int main(){
    
    char commands[100];
    char *args[10];
    printf("Justin's Terminal\n===========================================\n");
    printf("Please begin running Linux shell commands:\n");

    while(1){
        printf("> ");
        commands[100];
        fgets(commands, sizeof(commands), stdin);
        if(strcmp(commands, "exit\n") == 0){
            exit(0);
        }
        commands[strcspn(commands, "\n")] = 0;
        tokenize(commands, args);

        if(strcmp(args[0], "cd") == 0){
            chdir(args[1]);
            continue;
        }

        pid_t pid;
        if((pid = fork()) == 0){
            execvp(args[0], args);
        }
        waitpid(pid, NULL, 0);
    }
    
    return 0;
}