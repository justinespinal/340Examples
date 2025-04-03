#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

void createMatrix(FILE *file, int **matrix, int n){
    char buffer[256];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fgets(buffer, sizeof(buffer), file) == NULL) {
                fprintf(stderr, "Unexpected end of file while reading matrix\n");
                exit(1);
            }
            sscanf(buffer, "%d", &matrix[i][j]);
        }
    }
}

int main(int argc, char *argv[]){
    
    if(argc < 6 || argc > 6){
        printf("Usage: %s <numThreads> <N> <filename for A> <filename for B> <filename for C>\n", argv[0]);
        return 0;
    }

    int numThreads = atoi(argv[1]);
    if(numThreads < 1 || numThreads > 8){
        printf("Number of threads must be between 1 and 8\n");
        return 0;
    }

    int n = atoi(argv[2]);
    if(n < 1){
        printf("N must be positive");
        return 0;
    }

    FILE *fileA = fopen(argv[3], "r");
    if(!fileA){
        printf("First file does not exist");
        return 0;
    }
    int **matrixA = malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++) {
        matrixA[i] = malloc(n * sizeof(int));
    }   
    createMatrix(fileA, matrixA, n);

    FILE *fileB = fopen(argv[4], "r");
    if(!fileB){
        printf("Second file does not exist");
        return 0;
    }
    int **matrixB = malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++) {
        matrixB[i] = malloc(n * sizeof(int));
    }   
    createMatrix(fileB, matrixB, n);

    FILE *fileC = fopen(argv[5], "w");
    if(!fileC){
        printf("Third file does not exist");
        return 0;
    }
    int **matrixC = malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++) {
        matrixC[i] = malloc(n * sizeof(int));
    }   

    printf("%d", matrixA[0][1]);

    return 0;
}