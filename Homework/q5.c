#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Get the current stack pointer
    void* stack_pointer;
    __asm__("mov %%rsp, %0" : "=r"(stack_pointer)); // x86 architecture

    // Get the current heap pointer
    void* heap_pointer = sbrk(0);

    // Calculate the size of unused memory
    size_t unused_memory = (size_t)stack_pointer - (size_t)heap_pointer;

    printf("Stack Pointer: %p\n", stack_pointer);
    printf("Heap Pointer: %p\n", heap_pointer);
    printf("Unused Memory: %zu bytes (%.2f KB or %.2f MB)\n",
           unused_memory,
           (float)unused_memory / 1024,
           (float)unused_memory / (1024 * 1024));

    return 0;
}
