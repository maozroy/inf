#include <stdio.h> /* printf */

#include "calc.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

void TestAddAndSub();
void TestMultipleAndDivision();
void TestInvalidExp();
void TestDecimalNum();
void TestPow();
void TestDecimals();
void TestParenthesis();


int main()
{
	TestAddAndSub();
	TestMultipleAndDivision();
	TestInvalidExp();
	TestDecimalNum();
	TestPow();
	TestDecimals();

	TestParenthesis();
	
	return 0;
}

void TestAddAndSub()
{
	double res = 0;
	const char *exp1 = "16+9";
	const char *exp2 = "23-10";
	const char *exp3 = "300-90";
	const char *exp4 = "160-170-55+8";
	const char *exp5 = "43-54+87-765+65-87+3333";
	int status = 0;
	printf("TestAddAndSub\n");
	status = Calc(exp1, &res);
	TEST1(25, res);
	TEST1(0, status);
	Calc(exp2, &res);
	TEST1(13, res);
	Calc(exp3, &res);
	TEST1(210, res);
	Calc(exp4, &res);
	TEST1(-57, res);	
	Calc(exp5, &res);
	TEST1(2622, res);			
}

void TestMultipleAndDivision()
{
	double res = 0;
	const char *exp1 = "20*12";
	const char *exp2 = "1000/25";
	const char *exp3 = "654*473+47+44/3*3+2";
	const char *exp4 = "5/5-10*2+20/2";
	const char *exp5 = "78+65/54+12-32*32/32+21";
	int status = 0;
	printf("TestMultipleAndDivision\n");
	status = Calc(exp1, &res);
	TEST1(240, res);
	TEST1(0, status);
	Calc(exp2, &res);
	TEST1(40, res);
	Calc(exp3, &res);
	TEST1(309435, res);
	Calc(exp4, &res);
	TEST1(-9, res);
	Calc(exp5, &res);
	printf("result = %f\n", res); /* result = 80.203704 */ 
}

void TestInvalidExp()
{
	double res = 0;
	const char *exp1 = "*";
	const char *exp2 = "89+5+";
	const char *exp3 = "44+7-+7";
	const char *exp4 = "7854+4-5/0+54";
	const char *exp5 = "545+0^0-543/5/4";
	const char *exp6 = "545A+77";
	int status = 0;
	printf("TestInvalidExp\n");
	status = Calc(exp1, &res);
	TEST1(2, status);
	status = Calc(exp2, &res);
	TEST1(2, status);
	status = Calc(exp3, &res);
	TEST1(2, status);
	status = Calc(exp4, &res);
	TEST1(1, status);
	status = Calc(exp5, &res);
	TEST1(1, status);
	status = Calc(exp6, &res);
	TEST1(2, status);
}

void TestDecimalNum()
{
	double res = 0;
	const char *exp1 = "2.5*2";
	const char *exp2 = "6.75/2.25";
	int status = 0;
	printf("TestDecimalNum\n");
	status = Calc(exp1, &res);
	TEST1(0, status);
	TEST1(5, res);
	Calc(exp2, &res);
	TEST1(3, res);
}

void TestPow()
{
	double res = 0;
	const char *exp1 = "2^3^3";
	const char *exp2 = "2^8/2*2+9";
	const char *exp3 = "44^7/2/2*8-388";
	const char *exp4 = "2/34^0";
	const char *exp5 = "34^0+54/6+65";
	int status = 0;
	printf("TestPow\n");
	status = Calc(exp1, &res);
	TEST1(0, status);
	TEST1(134217728, res);
	Calc(exp2, &res);
	TEST1(265, res);
	Calc(exp3, &res);
	TEST1(638555618940, res);
	Calc(exp4, &res);
	TEST1(2, res);
	Calc(exp5, &res);
	TEST1(75, res);
}
/*
void Test()
{
	const char *exp1 = "((333+7))";
	const char *exp2 = "744+(5+6)-6)";
	const char *exp3 = "783+58-(58-(55+58))";
	const char *exp4 = "783+58-(58-55+58))";
	int state = 0;
	printf("Test\n");
	state = AreParenthesisBalancedIMP(exp1);
	TEST1(0, state);
	state = AreParenthesisBalancedIMP(exp2);
	TEST1(3, state);
	state = AreParenthesisBalancedIMP(exp3);
	TEST1(0, state);
	state = AreParenthesisBalancedIMP(exp4);
	TEST1(3, state);
}
*/
void TestParenthesis()
{
	double res = 0;
	const char *exp1 = "(7+5)";
	const char *exp2 = "(65+6-6(6)";
	const char *exp3 = "(66/2*(43-55*(7*6)^5))";
	const char *exp4 = "(66/2*(43-55*(7*6^5))";
	const char *exp5 = "2^((1+5/5*2)+2*(2+2-5/5))";
	int status = 0;
	printf("TestParenthesis\n");
	status = Calc(exp1, &res);
	TEST1(0, status);
	TEST1(12, res);
	status = Calc(exp2, &res);
	TEST1(2, status);
	Calc(exp3, &res);
	TEST1(-237204584661, res);
	status = Calc(exp4, &res);
	TEST1(2, status);
	Calc(exp5, &res);
	TEST1(512, res);
	
}	

void TestDecimals()
{
	double res = 0;
	const char *exp1 = "0.1+5";
	const char *exp2 = ".2(+.8)";
	const char *exp3 = "0.6*9";
	int status = 0;
	printf("TestDecimals\n");
	status = Calc(exp1, &res);
	TEST1(0, status);
	TEST1(5.1, res);
	status = Calc(exp2, &res);
	TEST1(0, status);
	Calc(exp3, &res);
	printf("%f\n",res);
	TEST1(5.400000, res);


}
	
