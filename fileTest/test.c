#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

void main(void)
{
	int fd1, fd2, flag1, flag2;
	fd1 = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
	flag1 = fcntl(fd1, F_GETFL);
	printf("flag = %d\n", flag1);
	fd2 = open("test.txt", O_CREAT | O_RDONLY | O_TRUNC, 0666);
	flag2 = fcntl(fd2, F_GETFL);
	flag1 = fcntl(fd1, F_GETFL);
	printf("flag1 = %d, flag2 = %d\n", flag1, flag2);
	printf("fd1 = %d, fd2 = %d\n", fd1, fd2);
}
