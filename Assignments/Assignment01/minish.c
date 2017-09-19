//
//  minish.c
//  CS 350 Assignment01
//
//  Created by Moaz Hamza on 9/11/17.
//  Copyright © 2017 Moaz Hamza. All rights reserved.
//

//printf, scanf
#include <stdio.h>
// fork
#include <unistd.h>
// exit
#include <stdlib.h>
// strcmp
#include <string.h>
// wait
#include <sys/wait.h>

#define MAX_LEN 1023
#define FORK_FAILED -1
#define IN_CHILD 0
#define MAX_NUM_ARGS 50
#define EXEC_FAILED -1


// Handle the signal

int main() {
    
    // Create a variable to recieve the input command.
    char *command_input = malloc(MAX_LEN);
    
    while(1){

        // Ask for the input
        // Recieve input, place into command_input
        printf ("minish>");
        fgets(command_input, MAX_LEN, stdin);
        if(strcmp(command_input, "exit\n") == 0){
            break;
        }
        
        command_input[strcspn(command_input, "\n")] = 0;
        char *arguments[MAX_NUM_ARGS];
        int i=0;
        char *currArg;
        
        currArg = strtok(command_input, " ");
        while(currArg){
            arguments[i] = currArg;
            i++;
            currArg = strtok(NULL, " " );
        }
        unsigned char background = 0;
        if(strcmp(arguments[i-1], "&") == 0) {
            background = 1;
            arguments[i-1] = NULL;
        }
        arguments[i] = NULL;
        // Fork and save the return value in pid
        pid_t pid = fork();
        int status = 0;
        
        // Error handle the fork
        if (pid == FORK_FAILED) {
            fprintf(stderr, "Fork failed\n");
            free(command_input);
            exit(1);
        }
        


        // Run the command in the child
        if (pid == IN_CHILD){
            // exec command
            if(execvp(arguments[0], arguments) == EXEC_FAILED) {
                fprintf(stderr, "Exec Failed\n");
                free(command_input);
                exit(2);
            }
        }
        
        // Wait for child to be finished in parent. pid is the child's proccess ID
        else{
            if(!background) waitpid(pid, &status, 0); 
        }
        
    }
    free(command_input);
    return 0;
}

