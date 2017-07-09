#include<stdio.h>
#include<sys/ipc.h>
#include<sys/un.h>

int main(void)
{
	printf("stdin is ");
	if (stdin->_flags & _IO_UNBUFFERED)
		printf("unbuffered\n");
	else if (stdin->_flags & _IO_LINE_BUF)
		printf("line buffered\n");
	else
		printf("full buffered\n");

	printf("stdout is ");
	if (stdout->_flags & _IO_UNBUFFERED)
		printf("unbuffered\n");
	else if (stdout->_flags & _IO_LINE_BUF)
		printf("line buffered\n");
	else
		printf("full buffered\n");

	printf("stderr is ");
	if (stderr->_flags & _IO_UNBUFFERED)
		printf("unbuffered\n");
	else if (stderr->_flags & _IO_LINE_BUF)
		printf("line buffered\n");
	else
		printf("full buffered\n");
	return 0;
}
