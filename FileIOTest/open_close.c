/*
 * open_close.c
 *
 *  Created on: May 4, 2017
 *      Author: zhaocc
 */
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
int main(void) {
	FILE *fp, *fp1;
	int fd;
	if ((fp = fopen(
			"/home/zhaocc/sourceCode/linux c/fileTest/StreamTest/hello.txt",
			"a+")) == NULL) {
		printf("fopen failed");
		exit(1);
	}

	if ((fd = open(
			"/home/zhaocc/sourceCode/linux c/fileTest/StreamTest/hello1.txt",
			O_RDWR)) == -1) {
		printf("open failed");
		exit(1);
	}

	if ((fp1 = fdopen(fd, "r")) == NULL) {
		printf("fopen failed");
		exit(1);
	}

//	char c = '\0';
//	while ((c = fgetc(fp1)) != EOF) {
//		if (fputc(c, stdout) == EOF) {
//			printf("fputc failed\n");
//			exit(1);
//		}
//		if (fputc(c, fp) == EOF) {
//			printf("fputc failed\n");
//			exit(1);
//		}
//	}

	char buf[1024];
	int n = 0;
	while((n = fread(buf, sizeof(char),1024, fp1)) > 0)
	{
		if(fwrite(buf,sizeof(char), n, fp) == -1)
		{
			printf("fputs failed");
		}
		fputs(buf, stdout);
	}
	fclose(fp);
	fclose(fp1);

	return 0;
}

