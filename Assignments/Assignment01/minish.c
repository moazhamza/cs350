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

struct process{
    pid_t pid;
    char *command_string;
    char *status_string;
};

typedef struct process process;

// Handle the signal

int main() {
    
    // Create a variable to recieve the input command.
    char *command_input = malloc(MAX_LEN);
    process backgroundedProcesses[100];
    unsigned int bp_count = 0;
    while(1){

        // Ask for the input
        // Recieve input, place into command_input
        printf ("minish>");
        fgets(command_input, MAX_LEN, stdin);
        if(strcmp(command_input, "exit\n") == 0){
            break;
        }

        if(strcmp(command_input, "listjobs\n") == 0){
            printf("List of backgrounded processes:\n");
            for(unsigned int i = 0; i < bp_count; i++){
                int statusNum;
                pid_t return_pid;
                if(strcmp(backgroundedProcesses[i].status_string, "FINISHED") != 0){
                    if((return_pid = waitpid(backgroundedProcesses[i].pid, &statusNum, WNOHANG)) == 0) backgroundedProcesses[i].status_string = "RUNNING";
                    else if (return_pid == backgroundedProcesses[i].pid) backgroundedProcesses[i].status_string = "FINISHED";
                    else{
                        fprintf(stderr, "Error in checking status of child process\n");
                        exit(3);
                    }
                }
                printf("Command %d with pid %d Status: %s\n", i+1, backgroundedProcesses[i].pid,backgroundedProcesses[i].status_string);
            }
            continue;
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
            else{
                process nextProcess = {pid, arguments[0], "RUNNING"};
                backgroundedProcesses[bp_count] = nextProcess;
                bp_count++;
            }
        }        
    }

    return 0;
}
