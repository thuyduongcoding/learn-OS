#include <stdio.h>
#include <pthread.h>

void* foo(void* args) {
    printf("He he %d\n", (int)args);
    //exit(NULL);      // question 1b
    return NULL;
}

int main() {

    int n_thread = 10;
    pthread_t* thread_ids = malloc(n_thread*sizeof(pthread_t)); 

    for (int i = 0; i < n_thread; i++)
         pthread_create(&thread_ids[i], NULL, foo, i);

    // exit(NULL); // question 1c
    for (int i = 0; i < n_thread; i++)
	 pthread_join(thread_ids[i], NULL);
}


