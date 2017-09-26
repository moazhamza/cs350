//
//  minish.c
//  CS 350 Assignment01
//
//  Created by Moaz Hamza on 9/11/17.
//  Copyright © 2017 Moaz Hamza. All rights reserved.
//

//printf, fprintf, fgets
#include <stdio.h>
// fork
#include <unistd.h>
// exit
#include <stdlib.h>
// strcmp
#include <string.h>
// wait
#include <sys/wait.h>
// signals
#include <signal.h>
#include <sys/types.h>

#define MAX_LEN 1023
#define FORK_FAILED -1
#define IN_CHILD 0
#define MAX_NUM_ARGS 50
#define EXEC_FAILED -1


pid_t currently_executing_pid = 0;

// Struct created to keep track of backgrounded processes
struct process{
    pid_t process_id;
    char *command_string;
    char *status_string;
};

typedef struct process process;

void remove_process(process *array, int index, int array_length){
       for(int i = index; i < array_length - 1; i++) array[i] = array[i + 1];
}

// Handle the signal
void signal_handler(int sigNum){
    if(sigNum == SIGINT){
        if(currently_executing_pid > 0){
            kill(currently_executing_pid, SIGTERM);
            currently_executing_pid = 0;
        }
    }
}

int main() {

    // Create a variable to recieve the input command.
    char *command_input = (char *)malloc(MAX_LEN);

    // Array of backgrounded processes
    process backgroundedProcesses[100];
    // Array pointer
    unsigned int bp_count = 0;

    // SIGNAL registering
    if (signal(SIGINT, signal_handler) == SIG_ERR){
        fprintf(stderr, "Unable to catch SIGINT\n");
    }

    if (signal(SIGTERM, signal_handler) == SIG_ERR){
        fprintf(stderr, "Unable to catch SIGTERM\n");
    }
    while(1){
        // Ask for the input
        // Recieve input, place into command_input
        printf ("minish>");
        fgets(command_input, MAX_LEN, stdin);

        // If command entered is "exit", then break out of shell. End program
        if(strcmp(command_input, "exit\n") == 0){
            if(bp_count > 0){
                for(unsigned int i=0; i<bp_count; i++) kill(backgroundedProcesses[i].process_id, SIGINT);
            }
            break;
        }

        // If command entered is "listjobs", list all backgrounded processes
        if(strcmp(command_input, "listjobs\n") == 0){
            printf("List of backgrounded processes:\n");
            // Loop through the backgrounded processes
            for(unsigned int i = 0; i < bp_count; i++){
                int statusNum;
                pid_t return_pid;
                // If the status of the process is already finished, don't recheck the status of child process
                if(strcmp(backgroundedProcesses[i].status_string, "FINISHED") != 0){
                    // Check status of child process, if it is 0, then it is still running
                    return_pid = waitpid(backgroundedProcesses[i].process_id, &statusNum, WNOHANG);
                    if(return_pid == 0) backgroundedProcesses[i].status_string = (char *) "RUNNING";
                    // If it is equal to the pid, then it is finished
                    else if (return_pid == backgroundedProcesses[i].process_id) backgroundedProcesses[i].status_string = (char *) "FINISHED";
                    // Else there was an error, exit with code 3
                    else{
                        fprintf(stderr, "Error in checking status of child process\n");
                        exit(3);
                    }
                }
                printf("Command %d with pid %d Status: %s\n", i+1, backgroundedProcesses[i].process_id,backgroundedProcesses[i].status_string);
            }
            // Once done re-ask for input
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
        // If command is fg, bring process to foreground
        
        int status = 0;
        if(strcmp(arguments[0], "fg") == 0){
            currently_executing_pid = atoi(arguments[1]);
            // TODO: Take out the process from the list and set a flag that says it's been foregrounded
            for(unsigned int k=0; k<bp_count; k++){
                if(backgroundedProcesses[k].process_id == currently_executing_pid) {
                    remove_process(backgroundedProcesses, k, 100);
                    bp_count--;
                    break;
                }
            }
            waitpid(atoi(arguments[1]), &status, 0);
            currently_executing_pid = 0;
            continue;
        }
        if(strcmp(arguments[i-1], "&") == 0) {
            background = 1;
            arguments[i-1] = NULL;
        }
        if(strcmp(arguments[0], "cd") == 0){
            if(chdir(arguments[1]) != 0){
                fprintf(stderr, "Changing directory failed\n");
            }
            continue;
        }
        arguments[i] = NULL;
        pid_t pid;
        // Fork and save the return value in pid
        pid = fork();
        
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
                fprintf(stderr, "Exec Failed\nPlease enter a valid command\n");
                free(command_input);
                exit(2);
            }
        }
        
        // Wait for child to be finished in parent. pid is the child's proccess ID
        else{

            if(!background){
                currently_executing_pid = pid;
                waitpid(pid, &status, 0); 
                currently_executing_pid = 0;
            }
            else{
                process nextProcess = {pid, arguments[0], (char*)"RUNNING"};
                backgroundedProcesses[bp_count] = nextProcess;
                bp_count++;
            }
        }        
    }

    return 0;
}
