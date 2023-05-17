#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define ARGC_MAX 10

//handler for ctrlC
void ctrlCHandler(int signal) {
        printf("Ctrl + c detected. Please use exit instead.");
}

int main(int argc, char **argv) {
        //register the ignore ctrl+c handler
        signal(SIGINT, ctrlCHandler);

        //initialize char pointerpointer for use with execv - needs to end with NULL
        char *args[ARGC_MAX+2];
        int ret;
        int pid = -1;

        do {
                //get current working directory and print result + ">"
                char cwd[1024];
                if(getcwd(cwd, sizeof(cwd)) != NULL) {
                        printf("%s> ", cwd);
                } else {
                        perror("getcwd");
                        exit(EXIT_FAILURE);
                }

                //make input possible
                char input[100];
                fgets(input, 100, stdin);

                if(input[0] == '\n') {
                        continue;
                }

                //replace \n with \0 for later use as single strings
                input[strcspn(input, "\n")] = '\0';

                // - transfer parameters -

                //to determin parameters
                char *delimiter = " ";

                //initialize and set correct program name
                char *ptr = strtok(input, delimiter);

                //tests -----------------------------------------
                //printf("ptr 0 = %s", ptr);
                //-----------------------------------------------

                //set correct parameters
                int i = 0;
                while(ptr != NULL && i < ARGC_MAX) {
                        args[i] = ptr;

                        //troubleshooting------------------------
                        //printf("ptr aktuell: %s \n", ptr);
                        //---------------------------------------

                        ptr = strtok(NULL, delimiter);
                        i++;
                }

                //set last element NULL
                args[i] = NULL;

                //test ------------------------------------------
                //printf("args[0] = %s", args[0]);
                //printf("args[1] = %s", args[1]);
                //-----------------------------------------------

                //check for cd instead of normal input
                if(strcmp(args[0], "cd") == 0) {
                        if(i > 0) {
                                if(chdir(args[1]) == -1) {
                                        perror("cdinput");
                                }
                        } else {
                                fprintf(stderr, "No Argument for cd was found!");
                        }

                        continue;
                }

                //Check for exit instead of normal input
                if(strcmp(args[0], "exit") == 0) {
                        exit(EXIT_SUCCESS);
                }

                //neuer Prozess für Programmaufruf
                pid = fork();

                //Fehlerbehandlung
                if(pid == -1) {
                        exit(EXIT_FAILURE);
                }

                //child
                if(pid == 0) {
                        //start correct program with correct parameters
                        execvp(args[0], args);
                        exit(EXIT_SUCCESS);
                }

                //parent
                else {
                        wait(&ret);
                }

        } while(1);
        return 0;
}
