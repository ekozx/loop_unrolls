/* Evan Kozliner
 * CSE 2431
 * Usage:
 * 	gcc shell.c
 * 	./a.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define BUFFER_SIZE 50
//static char buffer[BUFFER_SIZE];
int command_count = 13;
int current = 0;
char *history[10];
static char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
void handle_command(char inputBuffer[], char *args[], int length, int *background);

void handle_SIGINT() {
	int i = 10; 
	for( i; i >= 0; i--) {
		if (history[i]) {
			printf("\n%d: %s", i, history[i]);
		}
	}
	printf("COMMAND->");
	fflush(0);
}

/* Pre-execution handling done here. History of the command entered is stored, error checking
 * performed and tests for 'r' commands to use history**/
void setup(char inputBuffer[], char *args[],int *background)
{
    /* read what the user enters on the command line */
    int length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  

	if (!((inputBuffer[0] == 'r') && (inputBuffer[1] == '\n'))) {
		history[current] = strdup(inputBuffer);
		current = (current + 1) % 10;
	}

    if (length == 0)
		exit(0);            /* ^d was entered, end of user command stream */
    if (length < 0){
		perror("error reading the command");
		exit(-1);           /* terminate with error code of -1 */
    }
	handle_command(inputBuffer, args, length, background);
} 

/**
 * Reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters. setup() sets the args parameter as a 
 * null-terminated string.
 */
void handle_command(char inputBuffer[], char *args[], int length, int *background) {
	int i,      /* loop index for accessing inputBuffer array */
	start,  /* index where beginning of next command parameter is */
	ct;     /* index of where to place the next parameter into args[] */
    ct = 0;
    start = -1;
	char bufferCopy[MAX_LINE];
	strncpy(bufferCopy, inputBuffer, MAX_LINE);
    /* examine every character in the inputBuffer */
    for (i = 0; i < length; i++) { 
		switch (inputBuffer[i]){
		case ' ':
		case '\t' :               /* argument separators */
		    if(start != -1){
				args[ct] = &inputBuffer[start];    /* set up pointer */
				ct++;
		    }
		    inputBuffer[i] = '\0'; /* add a null char; make a C string */
		    start = -1;
		    break;
		    
		case '\n':                 /* should be the final char examined */
		    if (start != -1){
				args[ct] = &inputBuffer[start];     
				ct++;
		    }
		    inputBuffer[i] = '\0';
		    args[ct] = NULL; /* no more arguments to this command */
		    break;

		case '&':
		    *background = 1;
		    inputBuffer[i] = '\0';
		    break;
		    
		default :             /* some other character */
		    if (start == -1)
				start = i;
		} 
    }    
    args[ct] = NULL; /* just in case the input line was > 80 */
}

int run_commands() {
	int background = 0;
    char *args[MAX_LINE/2+1];/* command line (of 80) has max of 40 arguments */
	printf("COMMAND->");
	fflush(0);
	setup(inputBuffer, args, &background);       /* get next command */
	if (strcmp(args[0], "r") == 0)  {
		if (args[1] == NULL)  { // Execute latest command in history
			printf("r\n");
		} else if (strlen(args[1]) == 1){ // Execute command starting with args[1]
			printf("%s\n", args[1]);
		} else {
			execute_subprocess(inputBuffer, args, background);
		}
	} else {
		execute_subprocess(inputBuffer, args, background);
	}
}

/* the steps are:
 (1) fork a child process using fork()
 (2) the child process will invoke execvp()
 (3) if background == 0, the parent will wait, 
	otherwise returns to the setup() function. */
int execute_subprocess(char inputBuffer[], char * args[], int background) {
    int status;
	pid_t pid = fork(); // 1)
	if (pid == 0) { // 2)
		if (execvp(*args, args) < 0) {
			printf("execvp error!\n");
			return 1;
		}
	} else if (pid > 0) { 
		if (background == 0) { // 3)
			/* wait until the child is terminated*/
			while(wait(&status) != pid) {
				;
			}
		}
	} else { // failure 
		printf("fork() failed!\n");
		return 1;
	}
	
}

void initialize_history() {
	int i;
	for (i = 0; i < 10; i++) {
            history[i] = NULL;
	}
}

/* Inifinite loop for getting commands and testing for signals */
int main(void)
{
	initialize_history();
	signal(SIGINT, handle_SIGINT);
    while (1){            /* Program terminates normally inside setup */
		if (run_commands() == 1) 
			return 1;
    }
}
