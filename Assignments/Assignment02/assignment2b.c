#include <stdio.h>
#include <linux/time.h>
#include <unistd.h>
#define __NR_sys_my_xtime 326

int main(){
	struct timespec xtime;
	syscall(__NR_sys_my_xtime, &xtime);
	printf("%lu\n", xtime.tv_nsec);
}
