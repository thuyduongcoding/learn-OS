#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
void main() {
int val = 5;
int pid = fork();
if (pid == 0){
val++;
}
else {
val++;
}
printf("%d\n", val);
//printf("val address: %p\n", &val);
}
