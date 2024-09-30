#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
// Function to handle the SIGINT signal (Ctrl+C)
void handle_sigint(int sig) {
printf("We handle the interrupt %d here\n", sig);
exit(0); // try replacing this with something else, e.g. printf("Hello!")
}
int main() {
// Set up the SIGINT signal handler
signal(SIGINT, handle_sigint);
// Main loop
while (1) {
printf("Running... Press Ctrl+C to interrupt.\n");
sleep(1); // Sleep for 1 second
}
return 0;
}
