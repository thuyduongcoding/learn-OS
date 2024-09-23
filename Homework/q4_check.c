#include <stdio.h>

int main() {
    void* stack_pointer;
    void* program_counter;

    // Check architecture and use appropriate inline assembly
#ifdef __x86_64__ // For x86 64-bit
    // Inline assembly to get the stack pointer (SP)
    __asm__("mov %%rsp, %0" : "=r"(stack_pointer)); // x86

    // Inline assembly to get the program counter (RIP)
    __asm__("lea (%%rip), %0" : "=r"(program_counter)); // x86
#elif defined(__arm__) || defined(__aarch64__) // For ARM
    // Inline assembly to get the stack pointer (SP)
    __asm__("mov %0, sp" : "=r"(stack_pointer)); // ARM

    // Inline assembly to get the program counter (PC)
     __asm__("mov %0, pc" : "=r"(program_counter)); // ARM
#else
    #error "Unsupported architecture"
#endif

    // Print the values of SP and PC
    printf("Stack Pointer (SP): %p\n", stack_pointer);
    printf("Program Counter (PC): %p\n", program_counter);

    // Check if SP is greater than PC
    if (stack_pointer > program_counter) {
        printf("SP is greater than PC.\n");
    } else {
        printf("SP is not greater than PC.\n");
    }

    return 0;
}
