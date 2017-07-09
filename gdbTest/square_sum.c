#include<stdio.h>
int calculate(int a, int b);
int main(void)
{
	int num1, num2, result;
	while (1)
	{
		printf("Enter two integer, or use 0,0 to exit\n");
		scanf("%d%d", &num1, &num2);
		if (num1 == 0 && num2 ==0)
		{
			exit(0);
		}
		result = calculate(num1, num2);
		printf("result = %d\n", result);
	}
	return 0;
}

int calculate(int a, int b)
{
	return (a*a + b*b);
}
