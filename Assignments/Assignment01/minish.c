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


#define MAX_LEN 1023
#define FORK_FAILED -1
#define IN_CHILD 0

int main(int argc, const char * argv[]) {
    
    // Create a variable to recieve the input command.
    char command_input[MAX_LEN];
    
    while(1){
    
        // Ask for the input
        printf ("minish>");
        scanf ("%s", command_input);
    
        // Fork and save the return value in pid
        pid_t pid = fork();
    
        // Error handle the fork
        if (pid == FORK_FAILED) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 

        // Run the command in the child
        
        if (pid == IN_CHILD){
            // exec command
            
            // Exit successfully
            exit(0);
        }
        
        // Wait for child to be finished in parent. pid is the child's proccess ID
        if (pid > 0){
            
        }
    }
    return 0;
}
