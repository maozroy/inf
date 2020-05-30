 /********************************************
*	Author : Maoz Roytman			
*	Reviewer : 	Tamir Dayan
*	calc					
*	Date: Tue Dec 22 2019
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <math.h> /*pow*/
#include <string.h>

#include "calc.h"
#include "../stack/stack.h"

#define SIZE_OF_STACK 30

typedef enum states
{
	WAIT_FOR_NUM = 0,
	WAIT_FOR_OP = 1,
	ERROR = 2
}states;

typedef status_t (*func)(void *exp);
static status_t ErrorIMP(void *exp);
static status_t PushToNumIMP(void *exp);
static status_t PushToOPIMP(void *exp);
static status_t PushToOPNoPrecedenceIMP(void *exp);
static status_t EndIMP(void *exp);
static status_t PushCloseParanthesisIMP(void *exp);

typedef double (*calc_func)(double num1, double num2, status_t *status);
static double AdditionIMP(double num1, double num2, status_t *status);
static double MultiplyIMP(double num1, double num2, status_t *status);
static double SubtractIMP(double num1, double num2, status_t *status);
static double DivisionIMP(double num1, double num2, status_t *status);
static double PowerIMP(double num1, double num2, status_t *status);

static status_t PerformCalcAndPushToStackIMP(char op);
static void AreParanthesisValidIMP(char *exp);

typedef struct op_struct
{
	calc_func func;
	int table;
}op_struct_t;

typedef struct state_and_func
{
	func action_func;
	states next_state;
}state_and_func;

stack_t *num_stack = NULL;
stack_t *op_stack = NULL;

states current_state;
status_t current_status;

#define FIVE_NULL {NULL,0}, {NULL,0}, {NULL,0}, {NULL,0}, {NULL,0}
#define TEN_NULL FIVE_NULL, FIVE_NULL
#define FORTY_FIVE_NULL TEN_NULL, TEN_NULL, TEN_NULL, TEN_NULL, FIVE_NULL
#define TenPushToNumIMP \
		{PushToNumIMP, WAIT_FOR_OP}, {PushToNumIMP, WAIT_FOR_OP}, \
		{PushToNumIMP, WAIT_FOR_OP}, {PushToNumIMP, WAIT_FOR_OP}, \
		{PushToNumIMP, WAIT_FOR_OP}, {PushToNumIMP, WAIT_FOR_OP}, \
		{PushToNumIMP, WAIT_FOR_OP}, {PushToNumIMP, WAIT_FOR_OP}, \
		{PushToNumIMP, WAIT_FOR_OP}, {PushToNumIMP, WAIT_FOR_OP}, 
#define FiveErrorFunc {ErrorIMP, ERROR}, {ErrorIMP, ERROR}, {ErrorIMP, ERROR}, \
						{ErrorIMP, ERROR}, {ErrorIMP, ERROR},
#define TenErrorFunc FiveErrorFunc FiveErrorFunc
#define TwentryErrorFunc TenErrorFunc TenErrorFunc
#define FiftyErrorFunc TwentryErrorFunc TwentryErrorFunc TenErrorFunc
#define FortyFiveErrorFunc TwentryErrorFunc TwentryErrorFunc FiveErrorFunc

state_and_func ASCII[3][255] = {{TwentryErrorFunc TwentryErrorFunc  /*0-39*/
					{PushToOPNoPrecedenceIMP, WAIT_FOR_NUM}, /*40 '(' */
					{ErrorIMP, ERROR}, {ErrorIMP, ERROR}, /*41-42*/
					{PushToNumIMP, WAIT_FOR_OP}, {ErrorIMP, ERROR},  /*43-44 '+'*/
					{PushToNumIMP, WAIT_FOR_OP}, /*45 '-'*/
					{PushToNumIMP, WAIT_FOR_OP}, /*47 '.'*/
					{ErrorIMP, ERROR}, TenPushToNumIMP /*48-59*/
					FiftyErrorFunc FiftyErrorFunc FiftyErrorFunc  /*59-209*/
					TwentryErrorFunc TwentryErrorFunc FiveErrorFunc /*210-255*/
					},{ 
				 	{EndIMP, WAIT_FOR_NUM}, TwentryErrorFunc TwentryErrorFunc /*0-40*/ 
					{PushCloseParanthesisIMP, WAIT_FOR_OP}, /*41 ')'*/
					{PushToOPIMP, WAIT_FOR_NUM}, /* 42 '*'*/
					{PushToOPIMP, WAIT_FOR_NUM}, {ErrorIMP, ERROR}, /*43-34 '+'*/
					{PushToOPIMP, WAIT_FOR_NUM}, {ErrorIMP, ERROR}, /*45-46 '-'*/
					{PushToOPIMP, WAIT_FOR_NUM},  /*47 '/' */
					FortyFiveErrorFunc  {ErrorIMP, ERROR}, /*46-92*/
					{PushToOPNoPrecedenceIMP, WAIT_FOR_NUM}, /*93 '^' */
					FiftyErrorFunc FiftyErrorFunc FiftyErrorFunc  /*94-244*/
					TenErrorFunc},
					{FiftyErrorFunc FiftyErrorFunc FiftyErrorFunc
					FiftyErrorFunc FiftyErrorFunc FiveErrorFunc}
					}; /*245-255*/ 

op_struct_t OP_FUNCS[55] = {{NULL,0}, {NULL,0}, {MultiplyIMP, 3}, 
							{AdditionIMP, 2}, {NULL,0}, {SubtractIMP, 2},
							{NULL,0}, {DivisionIMP, 3}, FORTY_FIVE_NULL, 
							{NULL, 0}, {PowerIMP, 4}};
							
status_t Calc(const char *exp, double *res)
{
	char dummy = ']';	
	char prev_char = 0;
	double num = 0;
	size_t len = strlen(exp);
	assert(exp);
	assert(res);
	
	current_status = SUCCESS;
	current_state = WAIT_FOR_NUM;
	
	num_stack = StackCreate(len, sizeof(double));
	if (NULL == num_stack)
	{
		return ALLOC_FAIL;
	}
	op_stack = StackCreate(len, sizeof(char));
	if (NULL == op_stack)
	{
		StackDestroy(num_stack);
		return ALLOC_FAIL;
	}
	
	AreParanthesisValidIMP((char *)exp);
	StackPush(op_stack, &dummy);
	
	while (current_status == SUCCESS && *(char*)exp != '\0')
	{
		prev_char = *exp;
	
		if (current_state == WAIT_FOR_NUM)
		{
			if (*exp != '(')
			{
				num = strtod(exp, (char**)&exp);
				current_status = ASCII[current_state][(int)prev_char].action_func(&num);
			}
			else
			{
				current_status = ASCII[current_state][(int)prev_char].action_func(&prev_char);
				++exp;
			}
		}
		else
		{
			current_status = ASCII[current_state][(int)prev_char].action_func((char *)exp);
			++exp;
		}
		current_state = ASCII[current_state][(int)prev_char].next_state;
	}
	if (current_status == SUCCESS)
	{
		current_status = ASCII[current_state][(int)*exp].action_func((char *)exp);
		*res = *(double *)StackPeek(num_stack);
	}
	
	StackDestroy(num_stack);
	StackDestroy(op_stack);
	
	return current_status;
}
static status_t PushCloseParanthesisIMP(void *exp)
{
	char prev_op = 0;
	status_t status = SUCCESS;
	
	(void) exp;
	for (prev_op = *(char*)StackPeek(op_stack); 
		prev_op != '(';
		StackPop(op_stack), prev_op = *(char*)StackPeek(op_stack))
	{
		status = PerformCalcAndPushToStackIMP(prev_op);
	}
	
	StackPop(op_stack);
	
	return status;
}

static status_t ErrorIMP(void *exp)
{
	(void) exp;
	current_status = INVALID_EXP;
	
	return ERROR;
}

static status_t PushToNumIMP(void *exp)
{
	double num = *(double*)exp;
	
	StackPush(num_stack, &num);

	return SUCCESS;
}

static status_t PushToOPNoPrecedenceIMP(void *exp)
{
	StackPush(op_stack, (char*)exp);
	
	return SUCCESS;
}

static status_t PushToOPIMP(void *exp)
{
	char previous_operation = *(char*)(StackPeek(op_stack));
	status_t status = SUCCESS;
	
	while ((OP_FUNCS[*	(char *)exp-'('].table <= 
			OP_FUNCS[(int)previous_operation-'('].table) &&
			status == SUCCESS)
	{
		status = PerformCalcAndPushToStackIMP(previous_operation);
		StackPop(op_stack);
		previous_operation = *(char*)StackPeek(op_stack);
	}
	
	StackPush(op_stack, (char*)exp);
	
	return status;
}

static status_t EndIMP(void *exp)
{
	double *top = NULL;
	double *bottom = NULL;
	char *op = NULL;
	double result = 0;
	status_t status = SUCCESS;
	
	(void) exp;
	
	while (StackSize(num_stack) > 1)
	{
		op = StackPeek(op_stack);
		StackPop(op_stack);
		bottom = StackPeek(num_stack);
		StackPop(num_stack);
		top = StackPeek(num_stack);
		StackPop(num_stack);
		result = OP_FUNCS[(int)*op-'('].func(*top, *bottom, &status);
		StackPush(num_stack,&result);
	}
	
	return status;
}

static double AdditionIMP(double num1, double num2, status_t *status)
{
	(void) status;

	return num1 + num2;
}
static double MultiplyIMP(double num1, double num2, status_t *status)
{
	(void) status;

	return num1 * num2;
}
static double SubtractIMP(double num1, double num2, status_t *status)
{
	(void) status;

	return num1 - num2;
}
static double DivisionIMP(double num1, double num2, status_t *status)
{
	if (num2 == 0)
	{
		*status =  DIVISION_BY_ZERO;
		return 0;
	}

	return num1 / num2;
}

static double PowerIMP(double num1, double num2, status_t *status)
{
	if ((num1 == 0) && (num2 == 0))
	{
		*status =  INVALID_EXP;
		
		return 0;	
	}
	return pow(num1, num2);
}

static status_t PerformCalcAndPushToStackIMP(char op)
{
	double *top = NULL;
	double *bottom = NULL;
	double result = 0;
	status_t status = SUCCESS;
	
	bottom = StackPeek(num_stack);
	StackPop(num_stack);
	top = StackPeek(num_stack);
	StackPop(num_stack);
	
	result = OP_FUNCS[(int)op-'('].func(*top, *bottom, &status);
	StackPush(num_stack,&result);
	
	return status;
}

static void AreParanthesisValidIMP(char *exp)
{
	stack_t *paran_stack = (stack_t*)StackCreate(SIZE_OF_STACK, sizeof(char));
	if (paran_stack == NULL)
	{
		current_status = ALLOC_FAIL;
		return;
	}
	while('\0' != *exp)
	{
		if ('(' == *exp)
		{
			StackPush(paran_stack, exp);
		}
		else if (')' == *exp)
		{
			if(StackIsEmpty(paran_stack))
			{
				current_status = INVALID_EXP;
				StackDestroy(paran_stack);
				
				return;
			}
			else
			{
				StackPop(paran_stack);
			}
		}
		++exp;
	}
	
	if (StackIsEmpty(paran_stack))
	{
		current_status = SUCCESS;
		StackDestroy(paran_stack);
		return;
	}
	else
	{
		current_status = INVALID_EXP;
	}
	
	StackDestroy(paran_stack);
}
