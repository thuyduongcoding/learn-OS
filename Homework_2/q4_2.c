#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void main() {
int val = 5;
int pid;
if (pid = fork())
wait(&pid);
else
exit(val);
val++;
printf("%d\n", val);
}
