/*
 * main.c
 *
 *  Created on: Apr 7, 2017
 *      Author: zhaocc
 */
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#define FLAGS O_WRONLY|O_CREAT|O_TRUNC
#define MODES S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH

int main(void) {
	const char *pathname;
	int fd;
	char pn[30];
	printf("please enter the file path:");
	gets(pn);
	pathname = pn;
	if (fd = open(pathname, FLAGS, MODES) == -1) {
		printf("open file failed\n");
		exit(1);
	}
	printf("OK, open file successful\n");
	printf("fd=%d\n", fd);
	return 0;
}

