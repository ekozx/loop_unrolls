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

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define BUFFER_SIZE 50
//static char buffer[BUFFER_SIZE];
int command_count = 13;
int current = 0;
char *strs[10];

	void handle_SIGINT() {
		int i = 10; 
		printf("SIGNAL\n");
	//	for( i; i >= 0; i--) {
	//		if (strs[i]) {
	//			printf("%d %s\n", i, strs[i]);
	//			//write(STDOUT_FILENO, strs[i], strlen(buffer));
	//			//printf("hello\n");
	//		}
	//	}
		//write(STDOUT_FILENO, strs[i], strlen(buffer));
	//	printf("COMMAND->");
	//	fflush(0);
	}
	/**
	 * setup() reads in the next command line, separating it into distinct tokens
	 * using whitespace as delimiters. setup() sets the args parameter as a 
	 * null-terminated string.
	 */

	void setup(char inputBuffer[], char *args[],int *background)
	{
	    int length, /* # of characters in the command line */
		i,      /* loop index for accessing inputBuffer array */
		start,  /* index where beginning of next command parameter is */
		ct;     /* index of where to place the next parameter into args[] */
	    ct = 0;
	    /* read what the user enters on the command line */
	    length = read(STDIN_FILENO, inputBuffer, MAX_LINE);  //printf("Storing: %s\n", inputBuffer);
		//strs[current] = strdup(inputBuffer);

	    start = -1;
	    if (length == 0)
		exit(0);            /* ^d was entered, end of user command stream */
	    if (length < 0){
		perror("error reading the command");
		exit(-1);           /* terminate with error code of -1 */
	    }
	    
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


	/* the steps are:
	 (1) fork a child process using fork()
	 (2) the child process will invoke execvp()
	 (3) if background == 0, the parent will wait, 
		otherwise returns to the setup() function. */
	int main(void)
	{
		 char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
	    int background;             /* equals 1 if a command is followed by '&' */
	    char *args[MAX_LINE/2+1];/* command line (of 80) has max of 40 arguments */
	    pid_t pid;
	    int status;
		int i;
		for (i = 0; i < 10; i++) {
                strs[i] = NULL;
		}

	    while (1){            /* Program terminates normally inside setup */
		struct sigaction handler;
		handler.sa_handler = handle_SIGINT;
		sigaction(SIGINT, &handler, NULL);
			background = 0;
			printf("COMMAND->");
			fflush(0);
			setup(inputBuffer, args, &background);       /* get next command */
			strs[current] = strdup(inputBuffer);
			//fgets(strs[current], MAX_LINE, stdin);
			current = (current + 1) % 10;

		//	pid = fork(); // 1)
		//	if (pid == 0) { // 2)
		//		if (execvp(*args, args) < 0) {
		//			printf("execvp error!\n");
		//			return 1;
		//		}
		//	} else if (pid > 0) { 
		//		if (background == 0) { // 3)
		//			/* wait until the child is terminated*/
		//			while(wait(&status) != pid) {
		//				;
		//			}
		//		}
		//	} else { // failure 
		//		printf("fork() failed!\n");
		//		return 1;
		//	}
	    }
	}
