#include <stdio.h>
int main(){
void* stack_pointer;
void* program_counter;

// Inline assembly to get the stack pointer (SP)
__asm__("mov %%rsp, %0" : "=r"(stack_pointer));
printf("Stack pointer is %p\n", stack_pointer);
// Inline assembly to get the program counter (RIP)
__asm__("lea (%%rip), %0" : "=r"(program_counter)); // x86
printf("Program counter is %p\n", program_counter);
}
