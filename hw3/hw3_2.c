#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100000  // Maximum size of the array

// Shared sorted array and its size
int sorted_array[MAX_SIZE];
int size = 0;

// Function to insert an integer into the sorted array
void insert_sorted(int data) {
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
    int n = *(int*)arg;
    for (int i = 0; i <= 2 * n; i += 2) {
        insert_sorted(i);
    }
    return NULL;
}

// Thread function to insert odd numbers from 2n + 1 to 1
void* insert_odd_numbers(void* arg) {
    int n = *(int*)arg;
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
    pthread_create(&even_thread, NULL, insert_even_numbers, &n);

    // Start the thread to insert odd numbers
    pthread_create(&odd_thread, NULL, insert_odd_numbers, &n);

    // Wait for both threads to finish
    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    // Print the final sorted array
    print_array();

    return 0;
}
