
// sleep : Implement the UNIX program sleep for xv6; 
// your sleep should pause for a user-specified number of ticks
// data : 4/10
// author : wpl

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	int n;
	if (argc != 2) {
		fprintf(2, "Please enter a number!\n");
		exit(1);
	}
	n = atoi(argv[1]);
	sleep(n);
	exit(0);
}