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
#include <ctype.h>

#include "calc.h"
#include "../stack/stack.h"

#define SIZE_OF_STACK 30

typedef enum states{WAIT_FOR_NUM = 0,
					WAIT_FOR_OP = 1,
					ERROR = 2} states;

typedef status_t (*func)(void *exp);

static status_t ErrorIMP(void *exp);
static status_t PushToNumIMP(void *exp);
static status_t PushToOPIMP(void *exp);
static status_t PushToOPNoPrecedenceIMP(void *exp);
static status_t EndIMP(void *exp);
static status_t PushCloseParanIMP(void *exp);

typedef double (*calc_func)(double num1, double num2, status_t *status);

static double AdditionIMP(double num1, double num2, status_t *status);
static double MultiplyIMP(double num1, double num2, status_t *status);
static double SubtractIMP(double num1, double num2, status_t *status);
static double DivisionIMP(double num1, double num2, status_t *status);
static double PowerIMP(double num1, double num2, status_t *status);

static status_t PerformCalcAndPushToStackIMP(char op);
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
					{PushCloseParanIMP, WAIT_FOR_OP}, /*41 ')'*/
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

typedef enum flag_input{OP = 0,
					NUM = 1
					} flag_input;
			
typedef union input
{
	double num;
	char op;
}input_t;	

typedef struct tagged_union
{
	input_t input;
	flag_input flag;
} t_union;
					
							
status_t Calc(const char *exp, double *res)
{
	char dummy = ']';	
	char current_char = 0;
	char prev_op = 0;
	void *prev_location = NULL;
	double num = 0;
	t_union tagged_input = {0};
	
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
	
	while (current_status == SUCCESS && *(char*)exp != '\0')
	{
		current_char = *exp;
		prev_op = *(char*)StackPeek(op_stack);
		prev_location = (void *)exp;
		num = strtod(exp, (char**)&exp);
		
		if (*((char*)exp-1) == '(' && 
			(current_char == '+' || current_char == '-') && 
			isdigit(*((char*)prev_location+1)) )
		{
			tagged_input.flag = NUM;
			tagged_input.input.num = num;
		}
		else
		{
			if (!(isdigit(current_char)) )
			{
				tagged_input.input.op = current_char;
				tagged_input.flag = OP;
				exp = prev_location;
				++exp;
			}
			else
			{
				tagged_input.flag = NUM;
				tagged_input.input.num = num;
			}
		}
		current_status = ASCII[current_state][(int)current_char].action_func(&tagged_input);
		current_state = ASCII[current_state][(int)current_char].next_state;
	}
	
	if (current_status == SUCCESS)
	{
		current_status = ASCII[current_state][*(char*)exp].action_func((char *)exp);
		*res = *(double *)StackPeek(num_stack);
	}
	
	StackDestroy(num_stack);
	StackDestroy(op_stack);
	
	return current_status;
}
static status_t PushCloseParanIMP(void *exp)
{
	char prev_op = 0;
	status_t status = SUCCESS;
	
	(void) exp;
	for (prev_op = *(char*)StackPeek(op_stack); 
		prev_op != '('; StackPop(op_stack), prev_op = *(char*)StackPeek(op_stack))
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
	t_union *input = exp;
	
	double num = input->input.num;
	
	StackPush(num_stack, &num);

	return SUCCESS;
}


static status_t PushToOPNoPrecedenceIMP(void *exp)
{
	t_union *input = exp;
	char op = input->input.op;

	StackPush(op_stack, &op);
	
	return SUCCESS;
}

static status_t PushToOPIMP(void *exp)
{
	char prev_op = *(char*)(StackPeek(op_stack));
	status_t status = SUCCESS;
	t_union *input = exp;
	char op = input->input.op;
	
	while ((OP_FUNCS[op-'('].table <= 
		OP_FUNCS[(int)prev_op-'('].table) &&
		status == SUCCESS)
	{
		status = PerformCalcAndPushToStackIMP(prev_op);
		StackPop(op_stack);
		prev_op = *(char*)StackPeek(op_stack);

	}

	StackPush(op_stack, &op);
	
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


