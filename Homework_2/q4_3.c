#include <stdio.h>
#include <unistd.h>
void main() {
fork();
fork();
fork();
printf("hello\n");
return 0;
}
