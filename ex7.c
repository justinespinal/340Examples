#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
    char *pSm = NULL;
    int *pi;
    char *pStr;
    int *pg;
    // Allocate 1024 bytes shared memory
    pSm = GetSharedMemory(1024);
    memset(pSm, 0, 1024);
    pi = (int *)pSm;
    pg = (int *)pSm + sizeof(int);
    pStr = (char *)(pSm + 2*sizeof(int));
    *pg++;
    if (fork() == 0){
        while (*pi == 0);
            printf("Str: <%s>, g: %d\n", pStr, *pg);
            *pg++;
            strcpy(pStr, "AA");
            *pi = 0;
    } else {
        strcpy(pStr, "Message");
        *pg++;
        *pi = 1;
        while (*pi == 1);
        printf("Str: <%s>, g: %d\n", pStr, *pg);
    } //end-else
} //end-main
