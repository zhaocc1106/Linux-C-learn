/*pen_close.c
 *
 *  Created on: May 4, 2017
 *      Author: zhaocc
*/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
	int main(void)
{
	FILE * fp;
	int fd;
	if((fp = fopen("hello.txt", "w+")) == NULL)
	{
		printf("fopen failed");
		exit(1);
	}
	fprintf(fp, "hello");

	return 0;
}


