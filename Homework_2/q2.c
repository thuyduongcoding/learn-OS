#include <stdio.h>
#include <unistd.h>

void forkthem(int n){
if (n > 0) {
fork();
forkthem(n-1);
}
}

void main(int argc, char ** argv){
forkthem(6);
sleep(300);
}
