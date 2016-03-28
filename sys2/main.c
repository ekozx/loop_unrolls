//
//  main.c
//  Shell
//
//  Created by Bitex Kuang on 4/2/15.
//  Copyright (c) 2015 Bitex. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "queue.h"

#define MAX_LINE 80

/** setup() reads in the next command line, separating it into
 distinct tokens using whitespaces as delimeters.
 setup() modifies the args parameter so that it holds pointers
 to the null-terminated strings that are the tokens in the most
 recent user command line as well as a NULL pointer, indicating
 the end of the argument list, which comes after the string
 pointers that have been assigned to args.*/
int setup(char inputBuffer[], char * args[], int * background);
/* Executes a command. */
void executeCommand(char command[], char * args[], int background);
/* Removes the spaces around a string. */
void trim(char * str);
/* Gets the argument list from a string. */
int getArgs(char * str, char ** arr, int * background);
/* Cleans the argument list. */
void cleanArgs(char ** arr);
/* Handles the SIGINT signal. */
void handleSIGINT(int sig);
/* Shows the history commands list. */
void showHistory();
/* Executes the most recent command in the history. */
void executeRecent();
/* Executes a command from the history. */
void executeHistory(char first);

Queue historyQueue;
char * args[MAX_LINE / 2 + 1]; /* command line arguments */

int main(void) {
    char inputBuffer[MAX_LINE]; /* buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    int status;
    
    pid_t pid;

    historyQueue = CreateQueue();

    //struct sigaction handler;
    //handler.sa_handler = handleSIGINT;
    //sigaction(SIGINT, &handler, NULL);
    
    signal(SIGINT, handleSIGINT);

    while (1) {
        background = 0;
        printf("COMMAND-> ");
        fflush(stdout);
        /* setup() calls exit() when Control-D is entered */
        if (setup(inputBuffer, args, &background) == 0) {
            continue;
        }

        if (strcmp(args[0], "r") == 0) {
            // execute history command
            if (args[1] == NULL) {
                // execute most recent command
                executeRecent();
            } else if (strlen(args[1]) == 1) {
                // execute a history command starting with given char
                executeHistory(args[1][0]);
            }
        } else {
            executeCommand(inputBuffer, args, background);
        }
    }
    
    return (0);
}

int setup(char inputBuffer[], char * args[], int * background) {
    if (fgets(inputBuffer, MAX_LINE, stdin) == NULL) {
        exit(0);
    }
    
    cleanArgs(args);
    trim(inputBuffer);
    return getArgs(inputBuffer, args, background);
}

void executeCommand(char command[], char * args[], int background) {
    pid_t pid = fork();
    int status;

    if (pid < 0) { // error
        fprintf(stderr, "Error creating process.\n");
        exit(1);
    } else if (pid == 0) { // child process
        execvp(args[0], args);
        exit(0);
    } else { // parent process
        if (background == 1) {
            waitpid(pid, &status, 0);
        }
    }

    // add the command into queue
    if (QueueLength(historyQueue) == 10) {
        Dequeue(historyQueue);
    }
    Enqueue(historyQueue, command);
}

void trim(char * str) {
    int i, length;
    for (i = (int) strlen(str) - 1; i >= 0; i --) {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
            break;
        }
        str[i] = 0;
    }
    
    i = 0;
    while (str[i] == ' ') {
        i ++;
    }
    if (i > 0) {
        length = (int) strlen(str + i);
        memmove(str, str + i, length);
        str[length] = 0;
    }
}

void cleanArgs(char ** arr) {
    int i = 0;
    while (arr[i] != NULL) {
        free(arr[i]);
        arr[i ++] = NULL;
    }
}

int getArgs(char * str, char ** arr, int * background) {
    int count = 0;
    char * pch;
    char newstr[MAX_LINE];

    * background = 0;

    if (strlen(str) == 0) {
        return 0;
    }

    strcpy(newstr, str);

    pch = strtok(newstr, " ");
    
    while (pch != NULL) {
        count ++;
        arr[count - 1] = (char *) malloc(MAX_LINE * sizeof(char));
        strcpy(arr[count - 1], pch);
        
        pch = strtok(NULL, " ");
    }
    
    if (strcmp("&", arr[count - 1]) == 0) {
        * background = 1;
        free(arr[count - 1]);
        arr[count - 1] = NULL;
    } else {
        * background = 0;
    }

    return count;
}

void handleSIGINT(int sig) {
    signal(SIGINT, handleSIGINT);
    
    printf("\n");
    showHistory();
    printf("COMMAND-> ");
    fflush(stdout);
}

void showHistory() {
    Node * node = QueueTail(historyQueue);
    int i = QueueLength(historyQueue);
    if (i == 0) {
        printf("No commands in the history.\n");
        return;
    }

    printf("History commands:\n");
    while (node != NULL && node != historyQueue) {
        printf(" %d. %s\n", i, node->elem);
        i --;
        node = node->prev;
    }
}

void executeRecent() {
    Node * node = QueueTail(historyQueue);
    char * command;
    int background;
    if (node != NULL && node != historyQueue) {
        command = node->elem;
        cleanArgs(args);
        getArgs(command, args, & background);
        executeCommand(command, args, background);
    } else {
        printf("No commands in the history.\n");
    }
}

void executeHistory(char first) {
    Node * node = QueueTail(historyQueue);
    char * command;
    int background;
    while (node != NULL && node != historyQueue) {
        if (first == node->elem[0]) {
            command = node->elem;
            cleanArgs(args);
            getArgs(command, args, & background);
            executeCommand(command, args, background);
            return;
        }
        node = node->prev;
    }
    printf("No such command in the history.\n");
}
