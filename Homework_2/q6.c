#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
	
int main(){
	int pid;
	if (pid =fork()) wait(NULL);
	else {
		execl("/bin/ls", "ls", NULL);
		printf("execl returned! errno is [%d]\n",errno);
		perror("The error message is :");
	}
	return 0;
}
