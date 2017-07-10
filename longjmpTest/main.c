#include<stdio.h>
#include<stdlib.h>
#include<setjmp.h>

static void func1(int, int, int, int, int);
static void func2();

static jmp_buf jmpbuffer;
static int globalval;

void main (void)
{
	int autoval;
	int register regival;
	volatile int volval;
	static int staval;

	globalval = 1;
	autoval = 2;
	regival = 3;
	volval = 4;
	staval = 5;

	if (setjmp(jmpbuffer) != 0) {
		printf("after longjmp\n");
		printf("globalval:%d, autoval:%d, regival:%d, volval:%d, staval:%d\n", globalval, autoval, regival, volval, staval);
		exit(0);
	}

	globalval = 91;
	autoval = 92;
	regival = 93;
	volval = 94;
	staval = 95;

	func1(globalval, autoval, regival, volval, staval);
	exit(0);
}


static void func1(int a, int b, int c, int d, int e)
{
	printf("in func1\n");
	printf("globalval:%d, autoval:%d, regival:%d,"
			" volval:%d, staval:%d\n", a, b, b, d, e);
	func2();
}

static void func2()
{
	longjmp(jmpbuffer, 1);
}
