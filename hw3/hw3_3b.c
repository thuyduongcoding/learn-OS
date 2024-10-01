#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000 // Matrix size

// Structure to pass arguments to the thread function
typedef struct {
    int** A;
    int** B;
    int** C;
    int start_row;
    int end_row;
} ThreadArgs;

// Thread function to calculate part of C
void* multiply_matrices_threaded(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    for (int i = args->start_row; i < args->end_row; i++) {
        for (int j = 0; j < N; j++) {
            args->C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                args->C[i][j] += args->A[i][k] * args->B[k][j];
            }
        }
    }
    return NULL;
}

// Function to initialize matrices with random integer values
void initialize_matrices(int** A, int** B) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;  // Random integers between 0 and 9
            B[i][j] = rand() % 10;
        }
    }
}

// Function to perform multi-threaded multiplication
void multi_threaded_mult(int** A, int** B, int** C, int k) {
    pthread_t* threads = malloc(k * sizeof(pthread_t));
    ThreadArgs* args = malloc(k * sizeof(ThreadArgs));
    int rows_per_thread = N / k;

    for (int i = 0; i < k; i++) {
        args[i].A = A;
        args[i].B = B;
        args[i].C = C;
        args[i].start_row = i * rows_per_thread;
        args[i].end_row = (i == k - 1) ? N : (i + 1) * rows_per_thread; // Handle last thread
        pthread_create(&threads[i], NULL, multiply_matrices_threaded, &args[i]);
    }

    for (int i = 0; i < k; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(args);
}

// Show a sample of matrix elements
void printMatrixSample(int** matrix, const char* name) {
    printf("Sample of matrix %s (top-left 5x5):\n", name);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to allocate memory for a matrix dynamically
int** allocate_matrix(int n) {
    int** matrix = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));
    }
    return matrix;
}

// Function to free the dynamically allocated matrix
void free_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    // Allocate matrices dynamically
    int** A = allocate_matrix(N);
    int** B = allocate_matrix(N);
    int** C = allocate_matrix(N);

    initialize_matrices(A, B);

    int k = atoi(argv[1]);
    if (k > N) k = N; // Prevent more threads than rows

    // Measure execution time
    multi_threaded_mult(A, B, C, k);


    // Free dynamically allocated matrices
    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C, N);
    return 0;
}
