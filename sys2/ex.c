#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#define BUFFER_SIZE 50
#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#include <signal.h>

static char buffer[BUFFER_SIZE];
static char inputBuffer[255];
/* the signal handler function */
void handle_SIGINT() {
	write(STDOUT_FILENO,buffer,strlen(buffer));

	// delete the exit(0) statement for this lab assignment.
}
void other_function(inputBuffer[]) {
	printf("im here too:\n");
	//fgets(inputBuffer, 255, stdin);
	int length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  //printf("Storing: %s\n", inputBuffer);
	if (length < 0) {
		perror("err");
		exit(-1);
	}
	printf("%s\n", inputBuffer);
}
int main(int argc, char *argv[])
{
	/* set up the signal handler */
	//struct sigaction handler;
	//handler.sa_handler = handle_SIGINT;
	//sigaction(SIGINT, &handler, NULL);
	signal(SIGINT, handle_SIGINT);
	strcpy(buffer,"Caught <ctrl><c>\n");
	/* wait for <control> <C> */
	while (1) {
		other_function(inputBuffer);
	}
	return 0;
}
