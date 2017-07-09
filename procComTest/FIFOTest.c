#include<stdio.h>
#include<stdlib.h>
//#include<errno.h>
//#include<sys/types.h>
//#include<sys/stat.h>
int main(int argc, char *argv[])
{
	mode_t mode = 0666;
	if (argc != 2)
	{
		printf("error\n");
		exit(1);
	}
	if (mkfifo(argv[1], mode) < 0)
	{
		perror("failed to mkfifo");
		exit(1);
	}
	else
	{
		printf("mkfifo successfully name:%s\n", argv[1]);
	}
	return 0;
}
