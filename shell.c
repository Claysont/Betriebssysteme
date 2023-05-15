#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

        //initialize char pointerpointer for use with execv - needs to end with NULL
        char *args[2];

        args[1] = NULL;

        int ret;
        int pid = -1;

        do {
                pid = fork();

                //Errorhandling
                if(pid == -1) {
                        exit(EXIT_FAILURE);
                }

                //child
                if(pid == 0) {
                        //standard output + >

                        //full path for program
                        args[0] = "/bin/pwd";
                        args[1] = NULL;
                        execv(args[0], args);
                        exit(EXIT_SUCCESS);
                }

                //parent
                else {
                        wait(&ret);
                        printf("> ");

                        //make input possible
                        char input[100];

                        fgets(input, 100, stdin);
                        printf("Test eingabe: %s", input);

                        //new array for execv
                        char *args2[10];
                        char help[] = "/bin/";

                        // - transfer parameters -

                        //to determin parameters
                        char *delimiter = " ";
                        char *ptr;

                        //initialize and set correct program name
                        ptr = strtok(input, delimiter);

                        printf("ptr 0 = %s", ptr);
                        strcat(help, ptr);
                        printf("help = %s", help);
                        args2[0] = help;
                        printf("args2[0] = %s", args2[0]);

                        printf("ptr 0 = %s", ptr);
                        printf("args2[0] = %s", args2[0]);

                        //set correct parameters
                        int i = 1;
                        /*while(ptr != NULL && i < 10) {
                                args2[i] = ptr;

                                //troubleshooting
                                printf("ptr aktuell: %s \n", ptr);

                                ptr = strtok(NULL, delimiter);
                                i++;
                        }*/

                        //set last element NULL
                        args2[i] = NULL;

                        printf("args2[0] = %s", args2[0]);
                        printf("args2[1] = %s", args2[1]);
                        //neuer Prozess für Programmaufruf

                        int pid2 = fork();

                        //Fehlerbehandlung
                        if(pid2 == -1) {
                                exit(EXIT_FAILURE);
                        }

                        //child
                        if(pid2 == 0) {

                                //start correct program with correct parameters
                                execvp(args2[0], args2);

                                exit(EXIT_SUCCESS);

                        }


                        //parent

                        else {

                                wait(&ret);
                        }

                }
        } while(1);

}
