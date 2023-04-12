
//description:
// Write a simple version of the UNIX xargs program: read 
// lines from the standard input and run a command for each line, 
// supplying the line as arguments to the command. Your solution
//  should be in the file user/xargs.c.
// data : 4/11
// author : wpl

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void copy(char **p1, char *p2) {
	*p1 = malloc(strlen(p2) + 1);
	strcpy(*p1, p2);
}

// i 下标
int readLine(char **pars, int i) {
	int d = 1024;
	char buf[d];
	int j = 0;
	// 读取1行
	while(read(0, buf + j, 1)) {
		// 按行划分
		if (buf[j] == '\n') {
			buf[j] = 0;
			break;
		}
		j++;
		if (j == d) {
			fprintf(2, "Prarmeters are too long !\n");
			exit(1);
		}
	}
	// 没有读取内容
	if (j == 0) {
		return -1;
	}
	// 按照空格划分
	int k = 0;
	while (k < j) {
		if (i > MAXARG) {
			fprintf(2, "Please enter more parameter!\n");
			exit(1);
		}
		// '   abc   sdf'
		// 忽略前置 space‘ ’
		while((k < j) && (buf[k] == ' ')) {
			k++;
		} 
		// 起始位置
		int l = k;
		while((k < j) && (buf[k] != ' ')) {
			k++;
		}
		buf[k++] = 0;
		copy(&pars[i], buf + l);
		i++;
	}
	return i;
}

int main (int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(2, "Please enter more parameter!\n");
		exit(1);
	} else {
		int i;
		char *pars[MAXARG];
		for (i = 1;i < argc;i++) {
			copy(&pars[i - 1], argv[i]);
		}

		int end;
		while ((end = readLine(pars, argc - 1)) != -1) {
			pars[end] = 0;
			if (fork() == 0) {
				exec(pars[0], pars);
				exit(1);
			} else {
				wait(0);
			}
		}
		exit(0);
	}

}