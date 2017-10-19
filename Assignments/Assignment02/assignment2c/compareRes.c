#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define N 100
#define MAX_LENGTH 1024

int main(){
	struct timeval gtodTimes[N];
	char *procClockTimes[N];

	for(int i=0; i < N; i++) procClockTimes[i] = malloc(1024);

	int fd = open("/dev/mytime_device", O_RDONLY);

	if(fd < 0) {
		fprintf(stderr, "Unable to open file\n");
		exit(fd);
	}

	for(int i=0; i < N; i++){
		gettimeofday(&gtodTimes[i], 0);
		int bytes_read = read(fd, procClockTimes[i], MAX_LENGTH);
		if(bytes_read <= 0) exit(bytes_read);
	}

	close(fd);

	for(int i=0; i < N; i++){
		printf("User gettimeofday: %ld %ld\nKernel: %s\n", gtodTimes[i].tv_usec, gtodTimes[i].tv_sec, procClockTimes[i]);
	}
	return 0;
}
