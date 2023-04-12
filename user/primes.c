//description:
// Write a concurrent version of prime sieve using pipes. 
// This idea is due to Doug McIlroy, inventor of Unix pipes. 
// The picture halfway down this page and the surrounding text 
// explain how to do it. Your solution should be in the file user/primes.c.
// data : 4/10
// author : wpl

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int *fd) {
	int p, d;
	// 关闭写
	close(fd[1]);
	if (read(fd[0], (void* )&p, sizeof(p)) != sizeof(p)) {
		fprintf(2, "Read fail!\n");
		exit(1);
	} 
	printf("prime %d\n", p);
	if (read(fd[0], (void *)&d, sizeof(d))) {
		int fd1[2];
		pipe(fd1);
		if (fork() == 0) {
			primes(fd1);
		} else {
			// 关闭读
			close(fd1[0]);
			do {
				if (d % p != 0) {
					write(fd1[1], (void *)&d, sizeof(d));
				}
			} while(read(fd[0], (void *)&d, sizeof(d)));
			// 关闭读
			close(fd[0]);
			// 关闭写
			close(fd1[1]);
			wait(0);
		}
	}
	exit(0);
}

int main () {
	int fd[2];
	int start = 2;
	int end = 35;
	pipe(fd);
	if (fork() == 0) {
		primes(fd);
	} else {
		close(fd[0]); // 思考： 为什么需要先关闭读端？
		for (int i = start;i <= end;i++) {
			if (write(fd[1], (void * )&i, sizeof(i)) != 4) {
				fprintf(2, "Write fail!\n");
				exit(1);
			}
		}
		close(fd[1]);
		wait(0);
	}
	exit(0);
}