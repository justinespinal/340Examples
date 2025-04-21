#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct ThreadParam {
    int tid;
    int **matrixA;
    int **matrixB;
    int **matrixC;
    int n;
    int start;
    int end;
} ThreadParam;

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

void multiplyMatrix(int **A, int **B, int **C, int n, int start, int end){

    for (int i=start; i<=end; i++){
        for (int j=0; j<n; j++){
            C[i][j] = 0;
            for (int k=0; k<n; k++)
                C[i][j] += A[i][k]*B[k][j];
        }
    } 
}

void* threadFunction(void* arg){
    ThreadParam *param = (ThreadParam*) arg;

    multiplyMatrix(param->matrixA, param->matrixB, param->matrixC, param->n, param->start, param->end);
    pthread_exit(NULL);
}

void writeToFile(FILE* file, int **matrix, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i == n-1 && j == n-1){
                fprintf(file, "%d", matrix[i][j]);
            }else{
                fprintf(file, "%d\n", matrix[i][j]);
            }
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
    
    int rows[numThreads+1];
    for(int i = 0; i <= numThreads; i++){
        rows[i] = i * (n/numThreads);
    }

    ThreadParam params[numThreads];
    int baseRowCount = n / numThreads;
    int leftover = n % numThreads;

    int currentRow = 0;
    for (int i = 0; i < numThreads; i++) {
        int rowsForThread = baseRowCount + (i < leftover ? 1 : 0);
        params[i].tid = i;
        params[i].matrixA = matrixA;
        params[i].matrixB = matrixB;
        params[i].matrixC = matrixC;
        params[i].n = n;
        params[i].start = currentRow;
        params[i].end = currentRow + rowsForThread - 1;
        currentRow += rowsForThread;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t threads[numThreads];
    for(int i = 0; i<numThreads; i++){
        pthread_create(&threads[i], NULL, threadFunction, &params[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    printf("Execution time: %.6f seconds\n", elapsed);

    writeToFile(fileC, matrixC, n);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(matrixA[i]);
        free(matrixB[i]);
        free(matrixC[i]);
    }
    free(matrixA);
    free(matrixB);
    free(matrixC);

    // Close file handles
    fclose(fileA);
    fclose(fileB);
    fclose(fileC);


    return 0;
}