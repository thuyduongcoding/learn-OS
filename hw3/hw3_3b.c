#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Create a new type for the thread argument
struct thread_arg {
    int** matA;
    int** matB;
    int** matC;
    int* rows;
};


void* multiply(void* args) {
    // Cast the argument to the thread_arg type
    struct thread_arg* data = (struct thread_arg*) args;

    // Matrix multiplicaion logic
    for (int i = 0; i < data->rows[0]; i++) {
        // Get the row indices given to the thread
        int r = data->rows[i];
        for (int c = 0; c < data->rows[0]; c++) {
            data->matC[r][c] = 0; 
            for (int k = 0; k < data->rows[0]; k++) {
                data->matC[r][c] += data->matA[r][k] * data->matB[k][c];
            }
        }
    }
    return NULL;
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
    int n = 3; // Matrix size
    int k = atoi(argv[1]); // Number of threads
    
    // Allocate memory for matA, matB and matC
    int** matA = (int**)malloc(n * sizeof(int*));
    int** matB = (int**)malloc(n * sizeof(int*));
    int** matC = (int**)malloc(n * sizeof(int*));
    
    // Initialize matrices
    for (int i = 0; i < n; i++) {
        matA[i] = (int*)malloc(n * sizeof(int));
        matB[i] = (int*)malloc(n * sizeof(int));
        matC[i] = (int*)calloc(n, sizeof(int)); //Initialize all matC elements to be 0
        
        // Fill with example values
        for (int j = 0; j < n; j++) {
            matA[i][j] = rand() % 10; // Random values for matA
            matB[i][j] = rand() % 10; // Random values for matB
        }
    }

    // Print the resulting matrix
    printf("Matrix A\n");
    printMatrix(matA, n);

    // Print the resulting matrix
    printf("Matrix B\n");
    printMatrix(matB, n);

    // Create an array of thread_arg type, each element store the data for each thread
    struct thread_arg args[k];
    pthread_t* thread_ids = (pthread_t*)malloc(k*sizeof(pthread_t));

    int rows_per_thread = n/k;
    int remaining_rows = n;

    for (int th = 0; th < k; th++) {
        // In the last thread, remaining rows might be smaller than the calculated number of rows per thread
        int no_rows = (remaining_rows < rows_per_thread) ? remaining_rows : rows_per_thread; 

        args[th].matA = matA;
        args[th].matB = matB;
        args[th].matC = matC;

        // Allocate memory for rows
        args[th].rows = malloc((no_rows + 1) * sizeof(int)); // +1 to store number of rows
        args[th].rows[0] = no_rows;

        // Assign row indices for each thread
        for (int i = 1; i <= no_rows; i++) {
            args[th].rows[i] = th * rows_per_thread + (i - 1);
        }

        // Create threads
        pthread_create(&thread_ids[th], NULL, multiply, &args[th]);
        
        remaining_rows -= no_rows;
        }

    // Wait for all threads to finish
    for (int i = 0; i < k; i++) {
        pthread_join(thread_ids[i], NULL);
        free(args[i].rows); // Free allocated memory for rows
    }
    
    // Print the resulting matrix
    printf("Matrix C\n");
    printMatrix(matC, n);

    freeMatrix(matA, n);
    freeMatrix(matB, n);
    freeMatrix(matC, n);
    return 0;

    }

    

