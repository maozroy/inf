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
#include <math.h>

#include "calc.h"
#include "../stack/stack.h"

#define SIZE_OF_STACK 30
#define DISTANT_TO_MULTIPLY 42

typedef enum states{WAIT_FOR_NUM = 0,
					WAIT_FOR_OP = 1,
					ERROR = 2,
					END = 3} states;

			
typedef status_t (*func)(void *exp);

static status_t ErrorIMP(void *exp);
static status_t PushToNumIMP(void *exp);
static status_t PushToOPIMP(void *exp);
static status_t PushToOPNoPrecedenceIMP(void *exp);
static status_t EndIMP(void *exp);
static status_t PushCloseParanIMP(void *exp);

typedef double (*calc_func)(double num1, double num2);

static double AdditionIMP(double num1, double num2);
static double MultiplyIMP(double num1, double num2);
static double SubtractIMP(double num1, double num2);
static double DivisionIMP(double num1, double num2);
static double PowerIMP(double num1, double num2);

static void PerformCalcAndPushToStackIMP(char op);
static void IsParanthesisValidIMP(char *exp);

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

stack_t *num_stack = NULL;
stack_t *op_stack = NULL;
states current_state;
status_t current_status;

state_and_func ASCII[2][255] = {{TwentryErrorFunc TwentryErrorFunc  /*0-39*/
					{PushToOPNoPrecedenceIMP, WAIT_FOR_NUM}, /*40 '(' */
					FiveErrorFunc {PushToNumIMP, WAIT_FOR_OP}, /*41-47 '.'*/
					{ErrorIMP, ERROR}, TenPushToNumIMP /*48-59*/
					FiftyErrorFunc FiftyErrorFunc FiftyErrorFunc  /*59-209*/
					TwentryErrorFunc TwentryErrorFunc FiveErrorFunc /*210-255*/
					},{ 
				 	{EndIMP, WAIT_FOR_NUM}, TwentryErrorFunc TwentryErrorFunc /*0-40*/ 
					{PushCloseParanIMP, WAIT_FOR_OP}, /*41 ')'*/
					{PushToOPIMP, WAIT_FOR_NUM}, /* 42 '*'*/
					{PushToOPIMP, WAIT_FOR_NUM}, {ErrorIMP, ERROR}, /*43-34 '+'*/
					{PushToOPIMP, WAIT_FOR_NUM}, {ErrorIMP, ERROR}, /*45-46 '-'*/
					{PushToOPIMP, WAIT_FOR_NUM},  /*47 '/' */
					FortyFiveErrorFunc {PushToOPNoPrecedenceIMP, WAIT_FOR_NUM}, /*48-94 '^' */
					FiftyErrorFunc FiftyErrorFunc FiftyErrorFunc  /*95-245*/
					TenErrorFunc}}; /*246-256*/ 

op_struct_t OP_FUNCS[55] = {{NULL,0}, {NULL,0}, {MultiplyIMP, 3}, 
							{AdditionIMP, 2}, {NULL,0}, {SubtractIMP, 2},
							{NULL,0}, {DivisionIMP, 3}, FORTY_FIVE_NULL, 
							{NULL, 0}, {PowerIMP, 4}};
							
							
status_t Calc(const char *exp, double *res)
{
	char dummy = ']';	
	char prev_char = 0;
	double num = 0;
	
	current_status = SUCCESS;
	current_state = WAIT_FOR_NUM;
	
	num_stack = StackCreate(SIZE_OF_STACK, sizeof(double));
	if (NULL == num_stack)
	{
		return ALLOC_FAIL;
	}
	op_stack = StackCreate(SIZE_OF_STACK, sizeof(char));
	if (NULL == op_stack)
	{
		return ALLOC_FAIL;
	}
	
	
	IsParanthesisValidIMP((char *)exp);
	StackPush(op_stack, &dummy);
	
	while (current_status == SUCCESS)
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
				current_status = ASCII[current_state][(int)prev_char].action_func(&exp);
				++exp;
			}
		}
		else
		{
			current_status = ASCII[current_state][(int)prev_char].action_func(&prev_char);
			++exp;
		}		
	current_state = ASCII[current_state][(int)prev_char].next_state;
	}
	
	
	if (!StackIsEmpty(num_stack))
	{
		*res = *(double *)StackPeek(num_stack);
	}
	StackDestroy(num_stack);
	StackDestroy(op_stack);
	
	return current_status;
}
static status_t PushCloseParanIMP(void *exp)
{
	char prev_op = 0;
	(void) exp;
	for (prev_op = *(char*)StackPeek(op_stack); 
		prev_op != '('; StackPop(op_stack), prev_op = *(char*)StackPeek(op_stack))
	{
		PerformCalcAndPushToStackIMP(prev_op);
	}
	StackPop(op_stack);
	
	return SUCCESS;
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
	char previous_operation = 0;
	
	if (!StackIsEmpty(op_stack))
	{
		previous_operation = *(char*)(StackPeek(op_stack));
	}
	
	while ((OP_FUNCS[*	(char *)exp-'('].table <= 
		OP_FUNCS[(int)previous_operation-'('].table) &&
		!StackIsEmpty(op_stack))
	{
		PerformCalcAndPushToStackIMP(previous_operation);
		StackPop(op_stack);
		previous_operation = *(char*)StackPeek(op_stack);

	}

	StackPush(op_stack, (char*)exp);
	
	return SUCCESS;
}

static status_t EndIMP(void *exp)
{
	double *top = NULL;
	double *bottom = NULL;
	char *op = NULL;
	double result = 0;
	(void) exp;
	
	while (StackSize(num_stack) > 1)
	{
		op = StackPeek(op_stack);
		StackPop(op_stack);
		bottom = StackPeek(num_stack);
		StackPop(num_stack);
		top = StackPeek(num_stack);
		StackPop(num_stack);
		result = OP_FUNCS[(int)*op-'('].func(*top, *bottom);
		StackPush(num_stack,&result);
	}
	
	return END;
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
	if (num2 == 0)
	{
		/*current_status =  DIVISION_BY_ZERO;*/
		return 0;
	}

	return num1 / num2;
}
static double PowerIMP(double num1, double num2)
{
	if ((num1 == 0) && (num2 == 0))
	{
		/*current_status =  DIVISION_BY_ZERO;*/
		return 0;	
	}
	return pow(num1, num2);
}


static void PerformCalcAndPushToStackIMP(char op)
{
	double *top = NULL;
	double *bottom = NULL;
	double result = 0;
	
	bottom = StackPeek(num_stack);
	StackPop(num_stack);
	
	top = StackPeek(num_stack);
	StackPop(num_stack);
	
	result = OP_FUNCS[(int)op-'('].func(*top, *bottom);
	
	StackPush(num_stack,&result);
}


static void IsParanthesisValidIMP(char *exp)
{
	stack_t *paran_stack = (stack_t*)StackCreate(SIZE_OF_STACK, sizeof(char));
	while(*exp != '\0')
	{
		if (*exp == '(')
		{
			StackPush(paran_stack, exp);
		}
		else if (*exp == ')')
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


