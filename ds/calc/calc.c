 /********************************************
*	Author : Maoz Roytman			
*	Reviewer : 	
*	calc					
*	Date: Tue Dec 17 17:23:22 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <stdio.h>

#include "calc.h"
#include "../stack/stack.h"

#define SIZE_OF_STACK 30

typedef enum states{WAIT_FOR_NUM = 0,
					WAIT_FOR_OP = 1,
					ERROR = 2,
					END = 3} states;
					

			
typedef states (*func)(const char **exp);
typedef double (*calc_func)(double num1, double num2);
states ErrorIMP(const char **exp);
states PushToNumIMP(const char **exp);
states PushToOPIMP(const char **exp);
states EndIMP(const char **exp);
static double AdditionIMP(double num1, double num2);
static double MultiplyIMP(double num1, double num2);
static double SubtractIMP(double num1, double num2);
static double DivisionIMP(double num1, double num2);
static double PowerIMP(double num1, double num2);
static void MoveOpStackToNumStackIMP();

typedef struct op_struct
{
	calc_func func;
	int table;
}op_struct_t;

#define FIVE_NULL {NULL,0}, {NULL,0}, {NULL,0}, {NULL,0}, {NULL,0}
#define TEN_NULL FIVE_NULL, FIVE_NULL
#define FORTY_SIX_NULL TEN_NULL, TEN_NULL, TEN_NULL, TEN_NULL, FIVE_NULL, {NULL,0}
#define TenPushToNum PushToNumIMP, PushToNumIMP, PushToNumIMP, \
PushToNumIMP, PushToNumIMP,  PushToNumIMP, PushToNumIMP, \
PushToNumIMP, PushToNumIMP, PushToNumIMP, 
#define FiveErrorFunc ErrorIMP, ErrorIMP, ErrorIMP, ErrorIMP, ErrorIMP,
#define TenErrorFunc FiveErrorFunc FiveErrorFunc
#define TwentryErrorFunc TenErrorFunc TenErrorFunc
#define FiftyErrorFunc TenErrorFunc TenErrorFunc TenErrorFunc TenErrorFunc TenErrorFunc

stack_t *num_stack = NULL;
stack_t *op_stack = NULL;

func ASCII[2][255] = {{TenErrorFunc TenErrorFunc TenErrorFunc TenErrorFunc FiveErrorFunc/*0-44*/
					ErrorIMP, ErrorIMP, ErrorIMP, TenPushToNum /*45-57*/
					FiftyErrorFunc FiftyErrorFunc TwentryErrorFunc ErrorIMP,  /*58-179*/
					ErrorIMP, TwentryErrorFunc TwentryErrorFunc /*180-220*/
					TwentryErrorFunc TenErrorFunc FiveErrorFunc}, 
				 	{EndIMP, TenErrorFunc TenErrorFunc TenErrorFunc TenErrorFunc /*0-40*/
					ErrorIMP, ErrorIMP, PushToOPIMP, ErrorIMP, /*41-45*/
					PushToOPIMP, FiftyErrorFunc FiftyErrorFunc /*46-146*/
					FiftyErrorFunc FiftyErrorFunc FiveErrorFunc /*147-252*/
					ErrorIMP, ErrorIMP, ErrorIMP, ErrorIMP}}; /*252-255*/ 

op_struct_t OP_FUNCS[53] = { {MultiplyIMP, 3}, {AdditionIMP, 2}, {NULL,0}, {SubtractIMP, 2},
							{NULL,0}, {DivisionIMP, 3}, FORTY_SIX_NULL, {PowerIMP, 4}};
							
							
status Calc(const char *exp, double *res)
{
	states current_state = WAIT_FOR_NUM;
	
	num_stack = StackCreate(SIZE_OF_STACK, sizeof(double));
	op_stack = StackCreate(SIZE_OF_STACK, sizeof(char));
	
	
	while ((current_state != ERROR) && (current_state !=  END) )
	{
		current_state = ASCII[current_state][(int)*exp](&exp);
	}
	
	*res = *(double *)StackPeek(num_stack);
	
	StackDestroy(num_stack);
	StackDestroy(op_stack);
	
	
	return SUCCESS;
}


states ErrorIMP(const char **exp)
{
	(void) exp;
	printf("char %c ERROR", **exp);
	
	return ERROR;
}

states PushToNumIMP(const char **exp)
{
	double num = strtod(*exp, (char **)exp);
	
	StackPush(num_stack, &num);

	return WAIT_FOR_OP;
}

static double AdditionIMP(double num1, double num2)
{
	return num1 + num2;
}
static double MultiplyIMP(double num1, double num2)
{
	return num1 * num2;
}
static double SubtractIMP(double num1, double num2)
{
	return num1 - num2;
}
static double DivisionIMP(double num1, double num2)
{
	return num1 / num2;
}
static double PowerIMP(double num1, double num2)
{
	/*return pow(num1, num2);*/
}

states PushToOPIMP(const char **exp)
{
	StackPush(op_stack, *exp);
	++(*exp);
	
	return WAIT_FOR_NUM;
}

states EndIMP(const char **exp)
{
	double *top = NULL;
	double *bottom = NULL;
	char *op = NULL;
	double result = 0;
	(void) exp;

	
	MoveOpStackToNumStackIMP();
	
	while (StackSize(num_stack) > 1)
	{
		op = StackPeek(num_stack);
		StackPop(num_stack);
		bottom = StackPeek(num_stack);
		StackPop(num_stack);
		top = StackPeek(num_stack);
		StackPop(num_stack);
		result = OP_FUNCS[(int)*op-42].func(*top, *bottom);
		StackPush(num_stack,&result);
	}
	
	return END;
}

static void MoveOpStackToNumStackIMP()
{
	while(!StackIsEmpty(op_stack))
	{	
		StackPush(num_stack, StackPeek(op_stack));
		StackPop(op_stack);
	}
}

