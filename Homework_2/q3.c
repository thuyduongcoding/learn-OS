#include <stdio.h>
#include <unistd.h>

void main (int argc, char** argv) {
int child = fork();
int x = 5;
if (child == 0) {
x += 5;
printf("x of first child (final): %d\n", x);
}
else {
printf("x of parent after first fork: %d\n", x);
child = fork();
x += 10;
printf("x of second children: %d\n", x);
if (child){
printf("x of parent after second children fork: %d\n", x);
x += 5;
printf("x of parent (final): %d\n", x);
}
}
}
