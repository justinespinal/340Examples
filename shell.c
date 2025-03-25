#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

void tokenize(char input[], char *args[]){
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < 9) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void tokenizeBars(char input[], char *args[]){
    int i = 0;
    char *token = strtok(input, " | ");
    while (token != NULL && i < 9) {
        args[i++] = token;
        token = strtok(NULL, " | ");
    }
    args[i] = NULL;
}

void handle_redirect(int left, char leftSide[], char rightSide[]){
    if(strstr(leftSide, "<") != NULL || strstr(leftSide, ">") != NULL){
        // leftSide = handle_redirect();
    }
    if(strstr(rightSide, "<") != NULL || strstr(rightSide, ">") != NULL){
        // rightSide = handle_redirect();
    }

    if(left){
        pid_t pid;
        if((pid = fork()) == 0){
            int fd = open(rightSide, O_RDONLY);

            if (fd < 0) {
                perror("open failed");
                exit(1);
            }

            dup2(fd, 0);
            close(fd);
            char *args[10];
            tokenize(leftSide, args);
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        }
        waitpid(pid, NULL, 0);
    }else{
        pid_t pid;
        if((pid = fork()) == 0){
            int fd = open(rightSide, O_WRONLY);

            if (fd < 0) {
                perror("open failed");
                exit(1);
            }

            dup2(fd, 1);
            close(fd);
            char *args[10];
            tokenize(leftSide, args);
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        }
        waitpid(pid, NULL, 0);
    }
}

void handle_double_redirect(char *command){
    char *inputSymbol = strchr(command, '<');
    char *outputSymbol = strrchr(command, '>'); // last '>', strrchr looks from right

    *inputSymbol = '\0';
    *outputSymbol = '\0';

    char *commandPart = command;
    char *inputFile = inputSymbol + 1;
    char *outputFile = outputSymbol + 1;

    // Trim whitespace in-place
    while (*inputFile == ' ') inputFile++;
    while (*outputFile == ' ') outputFile++;
    while (*commandPart == ' ') commandPart++;

    // Remove trailing whitespace from inputFile (before outputSymbol used to begin)
    char *endInput = inputFile + strlen(inputFile) - 1;
    while (endInput > inputFile && (*endInput == ' ' || *endInput == '\t')) {
        *endInput-- = '\0';
    }

    commandPart[strcspn(commandPart, "\n")] = 0;
    inputFile[strcspn(inputFile, "\n")] = 0;
    outputFile[strcspn(outputFile, "\n")] = 0;

    pid_t pid;
    if((pid = fork()) == 0){
        int read_fd = open(inputFile, O_RDONLY);
        if (read_fd < 0) {
            perror("input file open failed");
            exit(1);
        }

        int write_fd = open(outputFile, O_WRONLY);
        if (write_fd < 0) {
            perror("input file open failed");
            exit(1);
        }

        dup2(read_fd, 0);
        close(read_fd);
        dup2(write_fd, 1);
        close(write_fd);

        char *args[10];
        tokenize(commandPart, args);
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

void execute(char commands[], char *args[]){ // need to finish for  | commands
    if(strchr(commands, '<') != NULL && strchr(commands, '>') != NULL){
        handle_double_redirect(commands);
    }
    else if (strchr(commands, '<') != NULL) {
        char *leftSide = strtok(commands, "<");
        char *rightSide = strtok(NULL, "<");

        leftSide[strcspn(leftSide, "\n")] = 0;
        rightSide[strcspn(rightSide, "\n")] = 0;
        while (*rightSide == ' ') rightSide++;

        handle_redirect(1, leftSide, rightSide);
    }
    else if (strchr(commands, '>') != NULL) {
        char *leftSide = strtok(commands, ">");
        char *rightSide = strtok(NULL, ">");

        leftSide[strcspn(leftSide, "\n")] = 0;
        rightSide[strcspn(rightSide, "\n")] = 0;
        while (*rightSide == ' ') rightSide++;

        handle_redirect(0, leftSide, rightSide);
    }
    else{
        execvp(args[0], args);
    }
}

int findBars(char commands[], int indexes[]) {
    int count = 0;
    for (int i = 0; commands[i] != '\0'; i++) {
        if (commands[i] == '|') {
            indexes[count++] = i;
        }
    }
    return count;
}

int countCommands(char *commands[]) {
    int count = 0;
    while (commands[count] != NULL) {
        count++;
    }
    return count;
}

int main(){
    
    char commands[100];
    char *args[10];
    printf("Justin's Terminal\n===========================================\n");
    printf("Please begin running Linux shell commands:\n");

    while(1){
        printf("> ");
        fgets(commands, sizeof(commands), stdin);
        commands[strcspn(commands, "\n")] = 0;

        if(strcmp(commands, "exit") == 0){
            exit(0);
        }

        if(strcmp(commands, "time") == 0){
            time_t t;
            time(&t);
            char *current_time = ctime(&t);
            printf("%s", current_time);
            continue;
        }

        tokenize(commands, args);

        if(strcmp(args[0], "cd") == 0){
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        }
        if(strchr(commands, '|') == NULL){
            printf("%s", commands);
            printf("no bar\n\n");
            pid_t pid;
            if ((pid = fork()) == 0) {
                execute(commands, args);
                perror("execution failed");
                exit(1); // exit the child if exec fails
            }
            waitpid(pid, NULL, 0);
        }else{
            char *barFuncs[10];
            tokenizeBars(commands, barFuncs);

            int numCommands = countCommands(barFuncs);
            int pipes[numCommands - 1][2]; // an array of pipes

            for (int i = 0; i < numCommands - 1; i++) {
                pipe(pipes[i]);
            }

            int i = 0;
            
            while(barFuncs[i] != NULL){
                char *args[10];
                tokenize(barFuncs[i], args);
                if(i == 0){
                    printf("%s, %s", barFuncs[i], args[0]);
                    pid_t pid;
                    if((pid = fork()) == 0){ 
                        dup2(pipes[i][1], 1);
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                        execute(barFuncs[i], args);
                    }
                    waitpid(pid, NULL, 0);
                    i += 1;
                    continue;
                }else if(barFuncs[i+1] == NULL){
                    pid_t pid;
                    if((pid = fork()) == 0){
                        dup2(pipes[i][0], 0);
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                        execute(barFuncs[i], args);
                    }
                    waitpid(pid, NULL, 0);
                    break;
                }else{
                    pid_t pid;
                    if((pid = fork()) == 0){ 
                        dup2(pipes[i][0], 0);
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                        dup2(pipes[i+1][1], 1);
                        close(pipes[i+1][0]);
                        close(pipes[i+1][1]);
                        execute(barFuncs[i], args);
                    }
                    waitpid(pid, NULL, 0);
                    close(pipes[i][0]);
                    i += 1;
                    close(pipes[i][1]);
                    continue;
                }
            }
        }
    }
    
    return 0;
}