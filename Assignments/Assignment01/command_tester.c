#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if(argc < 2) {
        fprintf(stderr, "Include number of seconds to sleep\n");
        exit(0);
    }
    sleep(atoi(argv[1]));
    printf("Slept for %s seconds\n", argv[1]);
}
