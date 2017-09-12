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

#define MAX_LEN 1023
#define FORK_FAILED -1
#define IN_CHILD 0
#define MAX_NUM_ARGS 50
#define EXEC_FAILED -1


// Handle the signal

int main() {
    
    // Create a variable to recieve the input command.
    char *command_input = malloc(MAX_LEN);
    
    // (Only in parent) Check to see if command is quit/exit. While it's not, keep running the shell
    while(strcmp(command_input, "exit\n") != 0){
        // Ask for the input
        // Recieve input, place into command_input
        printf ("minish>");
        fgets(command_input, MAX_LEN, stdin);
        
        
        // Fork and save the return value in pid
        pid_t pid = fork();
        
        // Error handle the fork
        if (pid == FORK_FAILED) {
            fprintf(stderr, "fork failed\n");
            free(command_input);
            exit(1);
        }
        
        // Run the command in the child
        
        if (pid == IN_CHILD){
            // Set up arguments
            char *arguments[MAX_NUM_ARGS];
            int i=0;
            printf("command_input: %s", command_input);
            while(sscanf(command_input, "%s", arguments[i]) > 0){
                fprintf(stderr, "%s\n", arguments[i]);
            }
            // exec command
            fprintf(stderr, "Executing: %s, %s\n", arguments[0], arguments[1]);
            if(execvp(command_input, arguments) == EXEC_FAILED) {
                fprintf(stderr, "Exec Failed\n");
                free(command_input);
                exit(2);
            }
        }
        
        // Wait for child to be finished in parent. pid is the child's proccess ID
        if (pid > 0){
            
        }
        
    }
    free(command_input);
    return 0;
}

