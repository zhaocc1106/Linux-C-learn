#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main(void)
{
	printf("uid:%d,gid:%d,euid:%d,egid:%d", getuid(), getgid(), geteuid(), getegid());
	return 0;
}
