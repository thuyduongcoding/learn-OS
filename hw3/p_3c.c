#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_arg {
    int** matA;
    int** matB;
    int** matC;
    int* rows; // Pointer to hold row indices
};

void* multiply(void* args) {
    struct thread_arg* data = (struct thread_arg*) args;

    // Matrix multiplication logic
    for (int i = 0; i < data->rows[0]; i++) {
        int r = data->rows[i];
        for (int c = 0; c < data->rows[0]; c++) {
            data->matC[r][c] = 0; // Initialize result cell
            for (int k = 0; k < data->rows[0]; k++) {
                data->matC[r][c] += data->matA[r][k] * data->matB[k][c];
            }
        }
    }

    return NULL; // Terminate thread
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int n = 3; // Size of matrices
    int k = atoi(argv[1]); // Number of threads

    if (k <= 0 || k > n) {
        fprintf(stderr, "Number of threads must be between 1 and %d\n", n);
        return 1;
    }

    int** matA = (int**)malloc(n * sizeof(int*));
    int** matB = (int**)malloc(n * sizeof(int*));
    int** matC = (int**)malloc(n * sizeof(int*));

    // Initialize matrices
    for (int i = 0; i < n; i++) {
        matA[i] = (int*)malloc(n * sizeof(int));
        matB[i] = (int*)malloc(n * sizeof(int));
        matC[i] = (int*)calloc(n, sizeof(int));

        // Fill with example values
        for (int j = 0; j < n; j++) {
            matA[i][j] = rand() % 10; // Random values for matA
            matB[i][j] = rand() % 10; // Random values for matB
        }
    }

    // Print matrices A and B
    printf("Matrix A:\n");
    printMatrix(matA, n);
    
    printf("Matrix B:\n");
    printMatrix(matB, n);

    struct thread_arg args[k];
    pthread_t* thread_ids = (pthread_t*)malloc(k * sizeof(pthread_t));

    int rows_per_thread = n / k;
    int remaining_rows = n;

    for (int th = 0; th < k; th++) {
        int no_rows = (remaining_rows > rows_per_thread) ? rows_per_thread : remaining_rows;

        args[th].matA = matA;
        args[th].matB = matB;
        args[th].matC = matC;

        // Allocate memory for rows and assign row indices
        args[th].rows = malloc((no_rows + 1) * sizeof(int)); // +1 to store number of rows
        args[th].rows[0] = no_rows;

        for (int i = 1; i <= no_rows; i++) {
            args[th].rows[i] = th * rows_per_thread + (i - 1);
        }

        pthread_create(&thread_ids[th], NULL, multiply, &args[th]);

        remaining_rows -= no_rows;
    }

    // Wait for all threads to finish
    for (int i = 0; i < k; i++) {
        pthread_join(thread_ids[i], NULL);
        free(args[i].rows); // Free allocated memory for rows
    }

    // Print the resulting matrix C
    printf("Matrix C:\n");
    printMatrix(matC, n);

    freeMatrix(matA, n);
    freeMatrix(matB, n);
    freeMatrix(matC, n);
    
    free(thread_ids); // Free allocated memory for thread IDs

    return 0;
}