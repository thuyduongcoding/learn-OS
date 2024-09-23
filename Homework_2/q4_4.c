#include <stdio.h>
#include <unistd.h>

void main(){
printf("A\n"); // Only be executed by the parent process
fork();
printf("B\n"); // Only be executed by the parent and first child process
fork();
printf("C\n"); // Be executed by all processes (4 in total)
}

