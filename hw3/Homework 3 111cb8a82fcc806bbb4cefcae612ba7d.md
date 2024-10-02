# Homework 3

## Q1:

```c
#include <stdio.h>
#include <pthread.h>

void* foo(void* args) {
    printf("He he %d\n", (int)args);
    //exit(NULL);      // question 1b
    return NULL;
}

int main() {

    int n_thread = 10;
    // Allocate memory for an array storing thread ids
    pthread_t* thread_ids = malloc(n_thread*sizeof(pthread_t)); 

    for (int i = 0; i < n_thread; i++)
         pthread_create(&thread_ids[i], NULL, foo, i);
         // &thread_ids[i]: pointer to the memory to store thread ID, 
         // the thread ID will be created and be stored in here.
         // NULL: parameters to set thread attributes, NULL means default attributes
         // foo: function to be executed
         // i: passed arg

    //exit(NULL); // question 1c

   // This loop waits for each thread to finish execution before proceeding.
    for (int i = 0; i < n_thread; i++)
			    pthread_join(thread_ids[i], NULL);
}
```

### How to create a thread in C?

```c
int pthread_create(pthread_t * thread, 
                   const pthread_attr_t * attr, 
                   void * (*start_routine)(void *), 
                   void *arg);
// thread: pointer to store the thread ID
// pthread_attr_t: pointer to structure used to define thread attributes (scheduling,...)
// start_routine: function executed by created thread, must return void*
// arg: argument of the start_routine, only allow 1 void* argument (use struct more multiple args)
```

### How to wait for the termination of a thread in C?

```c
int pthread_join(pthread_t th, 
                 void **thread_return);
// th: thread_id of the thread that the current thread is waiting
// thread_return: pointer to the location where the exit status of the th thread stored.
```

### a) Run this code multiple times. Do you get the same result? Why?

- First run:
    
    ![image.png](image.png)
    
- Second run:
    
    ![image.png](image%201.png)
    
- Third run:
    
    ![image.png](image%202.png)
    

**Answer:** Cannot get the same result because threads run concurrently so the order in which they execute is non-deterministic.

### b) Uncomment line 6, then run the code multiple times again. How does the result change? Why?

**Answer:** exit() function terminal the calling process immediately, not just the thread calling it. Thus, if one of the threads reach the exit() function, the whole program would be terminated while some threads have not finished their jobs. Thus, most of the results do not show all of 10 “He he” lines.

![image.png](image%203.png)

### c) Uncomment line 18 (while keeping line 6 commented), then run the code multiple times again. How does the result compare to part (a)? Why?

**Answer:** In part (a), all 10 print statements are displayed. However, in part (c), some print statements might be missing (only 7 and 8 as shown in the figures below). This is because the exit(NULL) at line 18 would terminate the whole program when the main code reaches that line. Thus, in some cases when the threads have not finished it jobs but the main program has reached the line 18, all the threads would be terminated. The `pthread_join` function will not be executed. Meanwhile, the print statements shown on the terminal are the threads have finish their jobs before the program reaches line 18.

![image.png](image%204.png)

![image.png](image%205.png)

## Q2:

### a) Read the code to understand how it works.

The code understand and explanation are shown within the comments below:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100000  // Maximum size of the array

// Shared sorted array and its size
int sorted_array[MAX_SIZE]; // Allocate the memory for an array of integers up to 100000
int size = 0; // initialize the current size of the array

// Function to insert an integer into the sorted array
void insert_sorted(int data) {
// This function find the position for the given integer 
// by continously comparing the given integer with numbers in array descendingly 
// until it can find the number smaller than it so it can be inserted after. 
// Meanwhile, the larger numbers would be continously be moved one position 
// to the right to create space for the new integer.

// Find the correct position to insert the new element
    int i = size - 1;
    while (i >= 0 && sorted_array[i] > data) {
        sorted_array[i + 1] = sorted_array[i];
        i--;
    }
    // Insert the new element into the correct position
    sorted_array[i + 1] = data;
    size++;  // Increase the size of the array
}

// Function to print the sorted array
void print_array() {
    for (int i = 0; i < size; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");
}

// Thread function to insert even numbers from 0 to 2n
void* insert_even_numbers(void* arg) {
		// Casting the argument into int type
    int n = *(int*)arg;
    // Insert integers in ascending order from 0 to 2n
    for (int i = 0; i <= 2 * n; i += 2) {
        insert_sorted(i);
    }
    return NULL;
}

// Thread function to insert odd numbers from 2n + 1 to 1
void* insert_odd_numbers(void* arg) {
    // Casting the argument into int type
    int n = *(int*)arg;
    // Insert integers in descending order from 2n+1 to 1
    for (int i = 2 * n + 1; i >= 1; i -= 2) {
        insert_sorted(i);
    }
    return NULL;
}

// Main function
int main(int argc, char* args[]) {
    int n = atoi(args[1]);

    //Create two threads
    pthread_t even_thread, odd_thread;

    // Start the thread to insert even numbers
    // *even_thread: pointer to interger value storing the thread id
    // NULL: default thread attribute
    // insert_even_numbers: function the event_thread executes
    // &n: pointer to the argument for insert_even_numbers function
    pthread_create(&even_thread, NULL, insert_even_numbers, &n);

    // Start the thread to insert odd numbers
    // *odd_thread: pointer to interger value storing the thread id
    // NULL: default thread attribute
    // insert_odd_numbers: function the event_thread executes
    // &n: pointer to the argument for insert_odd_numbers function
    pthread_create(&odd_thread, NULL, insert_odd_numbers, &n);

    // Wait for both threads to finish
    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    // Print the final sorted array
    print_array();

    return 0;
}
```

### (b) Compile and run the code with different values of n (e.g., 10, 20, 50, 100) to verify if the code works as expected.

![image.png](image%206.png)

~~The code works as expected.~~

- Running with large number might result in order error
- Those threads sharing the memory so the length of the array might be different → overlapping error
- **→ SYNCHRONIZATION**

### (c) Explain the results you observe

The results show the sorted integer array from 0 to (2n + 1) in ascending order, which is exactly what we one. It is because we have two threads that insert the odd numbers from 2n + 1 back to 1 and insert the even numbers from 0 up to 2n. We also have the `pthread_join` to ensure that the two threads finish their works.

## Q3: If A and B are $n \times n$ matrices, then the product matrix C is defined as:

$$
C(i,j) = \sum_{k=1}^{n} A(i,k) \cdot B(k,j)
$$

### a) Write a function to calculate C given matrices A and B using the formula above.

- Full code is in file hw3_3a.c

```c
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
```

### (b) Write a function to calculate C using k threads, where each thread calculates $\frac{n}{k}$ rows of matrix C.

- Full code is in file hw3_3b.c

```c
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
```

### (c) Randomize matrices A and B as $1000×1000$ matrices, and try different values of k (e.g., 2, 4, …, 20) to measure how multi-threaded programming improves the calculation speed.

- k = 2

![image.png](image%207.png)

- k = 4

![image.png](image%208.png)

- k = 8:

![Screenshot from 2024-10-01 22-24-16.png](Screenshot_from_2024-10-01_22-24-16.png)

- k = 16:

![Screenshot from 2024-10-01 22-25-10.png](Screenshot_from_2024-10-01_22-25-10.png)

- k = 20:

![Screenshot from 2024-10-01 22-25-33.png](Screenshot_from_2024-10-01_22-25-33.png)

The more threads used, it seems the faster the calculation. However, the differences are not very significant.