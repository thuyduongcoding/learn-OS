#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int** multiply(int** matA, int** matB, int n) {
    // Declare matC as an array of pointers that point to a row of integers in the matrix
    // Each element in C points to the first element of each row
    int** matC = (int**)malloc(n*sizeof(int*));

    // Allocate memory for the values of each row
    // Initialize value as 0 using calloc()
    for (int i =0; i < n; i++){
        matC[i] = (int*)calloc(n, sizeof(int));
    }

    // Matrix multiplicaion logic
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    return matC;
}

// Free the memory allocated for the matrix
void freeMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Show matrix elements
void printMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int n = 3;
    int** matA = (int**)malloc(n*sizeof(int*));
    int** matB = (int**)malloc(n*sizeof(int*));
    
    // Initialize A
    for(int i = 0; i < n; i++) {
        matA[i] = (int*)malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            matA[i][j] = rand();
        }
    }

    printf("Matrix A\n");
    printMatrix(matA, n);


    // Initialize B
    for(int i = 0; i < n; i++) {
        matB[i] = (int*)malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            matB[i][j] = rand();
        }
    }


    // Print the resulting matrix
    printf("Matrix B\n");
    printMatrix(matB, n);


    // Initialize C
    int** matC = multiply(matA, matB, n);
    
    // Print the resulting matrix
    printf("Matrix C\n");
    printMatrix(matC, n);

    freeMatrix(matA, n);
    freeMatrix(matB, n);
    freeMatrix(matC, n);
    return 0;
    
}