
#include "calc.h"
#include <stdio.h>
#define EXPRESION 5/5-10*2+20/2
#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

int main()
{

	char string[] = "5/5-10*2+20/2";
	char string2[] = "1-1/2*(2+1)";
	double result = 0;
	double calc = (double)38+10/1*5;
	printf("STATUE IS %d",Calc(string2, &result));
	printf("\nmy calc is   %f\n", result);
	printf("\nreal calc is %f", calc);
	
		return 0;
}

void TestPlusMultiplyEtc()
{
	char string[] = "5/5-10*2+20/2";
	char string2[] = "38+100/20*5";
	double result = 0;
	
	Calc(string, &result);

}
