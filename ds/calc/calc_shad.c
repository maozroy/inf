#include <string.h> /* strlen */
#include <stdlib.h> /* strtod */
#include <assert.h> /* assert */
#include <math.h> /* pow */

#include "calc.h"
#include "../../ds/stack/stack.h"

#define UNUSED(x) (void)(x) 
#define STATES 3
#define CHARS 256
#define NUM_ELEMENTS 30
#define OPS_NUM 94
#define TWO 2

typedef enum e_state {WAIT_FOR_NUM, WAIT_FOR_OP, ERROR} e_state;
typedef status_t (*action_func_t)(char char_i, double num, stack_t *num_stack, stack_t *op_stack);

typedef struct state
{
    e_state next_state;
    action_func_t action_func;
} state_t;

static status_t CheckParenthesisValidity(char *str);
static status_t PushOperand(char char_i, double num, stack_t *num_stack, stack_t *op_stack);
static status_t PushUnary(char char_i, double num, stack_t *num_stack, stack_t *op_stack);
static status_t PushOperator(char char_i, double num, stack_t *num_stack, stack_t *op_stack);
static status_t Error(char char_i, double num, stack_t *num_stack, stack_t *op_stack);
static status_t Calculate(stack_t *num_stack, stack_t *op_stack);
static status_t FoldDown(char char_i, double num, stack_t *num_stack, stack_t *op_stack);
static status_t FoldParenthesis(char char_i, double num, stack_t *num_stack, stack_t *op_stack);
static status_t DoNoth(char char_i, double num, stack_t *num_stack, stack_t *op_stack);

state_t LUT[CHARS][STATES] = {0};
double (*LUT_Ops[OPS_NUM])(double num1, double num2) = {0};
int LUT_precedence[OPS_NUM] = {0};
status_t (*LUT_action[TWO])(char char_i, double num, stack_t *num_stack, stack_t *op_stack) = {0};
int (*LUT_is_valid[OPS_NUM])(double num1, double num2) = {0};
void (*LUT_parser[CHARS])(char **str, double *num, char *char_i, stack_t *op_stack) = {0}; 

static double Addition(double num1, double num2);
static double Subtraction(double num1, double num2);
static double Multiplication(double num1, double num2);
static double Division(double num1, double num2);
static double Power(double num1, double num2);

static int IsDivisionValid(double num1, double num2);
static int IsPowerValid(double right_num, double left_num2);
static int ValidExp(double num1, double num2);
static int InvalidExp(double num1, double num2);
static int IsPrecedence(char char_i, stack_t *num_stack, stack_t *op_stack);
static int IsNotOpenParenthesisOrPowerOp(char char_i);

static void Number(char **str, double *num, char *char_i, stack_t *op_stack);
static void UnaryOrOperator(char **str, double *num, char *char_i, stack_t *op_stack);
static void Operator(char **str, double *num, char *char_i, stack_t *op_stack);
static void PopParenthesis(stack_t *op_stack);

static void InitAllLUTs();
static void InitLUT();
static void InitParserLUT();
static void InitOpsLUT();
static void InitPrecedenceLUT();
static void InitLUTAction();
static void InitLUTIsValid();

void InitAllLUTs()
{
    InitLUT();
    InitParserLUT();
    InitOpsLUT();
    InitPrecedenceLUT();
    InitLUTAction();
    InitLUTIsValid();
}
static void InitLUT()
{
    size_t i = 0;
    size_t j = 0;
    state_t error = {ERROR, &Error};
    state_t wait_for_num = {WAIT_FOR_OP, &PushOperand};
    state_t wait_for_op = {WAIT_FOR_NUM, &PushOperator};
    state_t wait_for_num_open_parenthesis = {WAIT_FOR_NUM, &PushOperator};
    state_t wait_for_num_close_parenthesis = {WAIT_FOR_OP, &FoldParenthesis};
    state_t wait_for_unary = {WAIT_FOR_OP, &PushUnary};
    state_t wait_for_num_end_of_str = {WAIT_FOR_NUM, &FoldDown};
    
    for (i = 0; i < CHARS; ++i)
    {
        for (j = 0; j < STATES; ++j)
        {
            LUT[i][j] = error;
        }
    }

    LUT['0'][0] = wait_for_num;
    LUT['1'][0] = wait_for_num;
    LUT['2'][0] = wait_for_num;
    LUT['3'][0] = wait_for_num;
    LUT['4'][0] = wait_for_num;
    LUT['5'][0] = wait_for_num;
    LUT['6'][0] = wait_for_num;
    LUT['7'][0] = wait_for_num;
    LUT['8'][0] = wait_for_num;
    LUT['9'][0] = wait_for_num;

    LUT['+'][1] = wait_for_op;
    LUT['+'][0] = wait_for_num;
    LUT['-'][1] = wait_for_op;
    LUT['-'][0] = wait_for_num;
    LUT['*'][1] = wait_for_op;
    LUT['/'][1] = wait_for_op;
    LUT['^'][1] = wait_for_op;
    LUT['('][0] = wait_for_num_open_parenthesis;
    LUT[')'][1] = wait_for_num_close_parenthesis;
    LUT['\0'][1] = wait_for_num_end_of_str;
}

static void InitParserLUT()
{
    size_t i = 0;

    for (i = 0; i < CHARS; ++i)
    {
        LUT_parser[i] = &Operator;
    }

    LUT_parser['0'] = &Number;
    LUT_parser['1'] = &Number;
    LUT_parser['2'] = &Number;
    LUT_parser['3'] = &Number;
    LUT_parser['4'] = &Number;
    LUT_parser['5'] = &Number;
    LUT_parser['6'] = &Number;
    LUT_parser['7'] = &Number;
    LUT_parser['8'] = &Number;
    LUT_parser['9'] = &Number;
    LUT_parser['+'] = &UnaryOrOperator;
    LUT_parser['-'] = &UnaryOrOperator; 
}

static void InitOpsLUT()
{
    LUT_Ops['+'] = &Addition;
    LUT_Ops['-'] = &Subtraction;
    LUT_Ops['*'] = &Multiplication;
    LUT_Ops['/'] = &Division;
    LUT_Ops['^'] = &Power;
}

static void InitPrecedenceLUT()
{
    LUT_precedence['('] = 0;
    LUT_precedence['+'] = 1;
    LUT_precedence['-'] = 1;
    LUT_precedence['*'] = 2;
    LUT_precedence['/'] = 2;
    LUT_precedence['^'] = 3;    
}

static void InitLUTAction()
{
    LUT_action[0] = &DoNoth;
    LUT_action[1] = &FoldDown;
}

static void InitLUTIsValid()
{
    LUT_is_valid['+'] = &ValidExp;
    LUT_is_valid['-'] = &ValidExp;
    LUT_is_valid['*'] = &ValidExp;
    LUT_is_valid['/'] = &IsDivisionValid;
    LUT_is_valid['^'] = &IsPowerValid;
    LUT_is_valid['('] = &InvalidExp;
}

status_t Calc(const char *exp, double *res)
{
    size_t num_of_elements = NUM_ELEMENTS;
    stack_t *num_stack = NULL;
    stack_t *op_stack = NULL;
    e_state state_i = WAIT_FOR_NUM;
    status_t status = SUCCESS;
    char *str_runner = NULL;
    size_t str_len = 0;

    assert(NULL != exp);

    InitAllLUTs();

    num_stack = StackCreate(num_of_elements, sizeof(double));
    if(NULL == num_stack)
    {
        return INVALID_EXP;
    }

    op_stack = StackCreate(num_of_elements, sizeof(char));
    if(NULL == op_stack)
    {
        StackDestroy(num_stack);

        return INVALID_EXP;
    }

    str_runner = (char *)exp;
    status = CheckParenthesisValidity(str_runner);

    while (('\0' != *str_runner) && (SUCCESS == status))
    {
        double num = 0.0;
        char char_i = 'v'; /* a sign, if changed, the expression is INVALID */
        state_t state = {0};

        state = LUT[(int)*str_runner][state_i];
        LUT_parser[*(char *)str_runner](&str_runner, &num, &char_i, op_stack);
        status = state.action_func(char_i, num, num_stack, op_stack);
        state_i = state.next_state;
    }
    
    if (SUCCESS == status)
    {
        double num = 0.0;
        char char_i = 'v';
        state_t state = {0};

        state = LUT[(int)*str_runner][state_i];
        LUT_parser[*(char *)str_runner](&str_runner, &num, &char_i, op_stack);
        status = state.action_func(char_i, num, num_stack, op_stack);
        
        *res = *(double *)StackPeek(num_stack);
        StackPop(num_stack);
    }

    StackDestroy(num_stack);
    StackDestroy(op_stack);

    return status;
}

static status_t CheckParenthesisValidity(char *str)
{
    size_t open_parenthesis_counter = 0;
    size_t close_parenthesis_counter = 0;
    status_t status = SUCCESS;
    char *str_runner = NULL;

    assert(NULL != str);

    str_runner = str;

    while ('\0' != *str_runner)
    {
        if ('(' == *str_runner)
        {
            ++open_parenthesis_counter;
        }
        else if (')' == *str_runner)
        {
            ++close_parenthesis_counter;
        }
        ++str_runner;
    }

    if (open_parenthesis_counter != close_parenthesis_counter)
    {
        status = INVALID_EXP;
    }

    return status;
}

static void Number(char **str, double *num, char *char_i, stack_t *op_stack)
{
    assert(NULL != str);
    assert(NULL != num);
    assert(NULL != char_i);

    UNUSED(char_i);

    *num = strtod(*str, str);
}

static void UnaryOrOperator(char **str, double *num, char *char_i, stack_t *op_stack)
{
    char *reference_str1 = NULL;
    char *reference_str2 = NULL;
    double num_ref1 = 0.0;
    double num_ref2 = 0.0;
    
    assert(NULL != str);
    assert(NULL != num);
    assert(NULL != char_i);

    if ((!StackIsEmpty(op_stack) && ('(' == *(*str - 1)) && ('(' == *(char *)StackPeek(op_stack))))
    {
        *num = strtod(*str, str);
    }
    else
    {
        *char_i = **str;
        ++(*str);
    }    
}

static void Operator(char **str, double *num, char *char_i, stack_t *op_stack)
{
    assert(NULL != str);
    assert(NULL != num);
    assert(NULL != char_i);

    UNUSED(num);

    *char_i = **str;
    if (**str != '\0')
    {
        ++(*str);
    }
}

static status_t PushOperand(char char_i, double num, stack_t *num_stack, stack_t *op_stack)
{
    assert(NULL != num_stack);
    assert(NULL != op_stack);

    UNUSED(char_i);
    UNUSED(op_stack);

    StackPush(num_stack, &num);

    return SUCCESS;
}

static status_t PushUnary(char char_i, double num, stack_t *num_stack, stack_t *op_stack)
{
    if (StackIsEmpty(op_stack) || '(' != *(char *)StackPeek(op_stack) || '\0' == char_i)
    {
        return INVALID_EXP;
    }

    return PushOperand(char_i, num, num_stack, op_stack);
}

static status_t PushOperator(char char_i, double num, stack_t *num_stack, stack_t *op_stack)
{
    status_t status = SUCCESS;
    int is_precedence = 0;
    int is_not_open_parenthesis_or_pow_op = 1;

    assert(NULL != num_stack);
    assert(NULL != op_stack);

    UNUSED(num_stack);
    
    is_not_open_parenthesis_or_pow_op = IsNotOpenParenthesisOrPowerOp(char_i);
    is_precedence = (is_not_open_parenthesis_or_pow_op && IsPrecedence(char_i, num_stack, op_stack));
    
    status = LUT_action[is_precedence](char_i, num, num_stack, op_stack);
    if (SUCCESS != status)
    {
        return INVALID_EXP;
    }
    
    StackPush(op_stack, &char_i);

    return status;
}

static int IsPrecedence(char char_i, stack_t *num_stack, stack_t *op_stack)
{
    int peek_of_op_stack = 0;
    int char_i_as_int = 0;

    assert(NULL != num_stack);
    assert(NULL != op_stack);

    if (StackIsEmpty(op_stack))
    {
        return 0;
    }
    peek_of_op_stack = *(char *)StackPeek(op_stack);
    char_i_as_int = char_i;

    return ( ('(' != *(char *)StackPeek(op_stack)) && 
    (LUT_precedence[peek_of_op_stack] >= LUT_precedence[char_i_as_int]));
}

static int IsNotOpenParenthesisOrPowerOp(char char_i)
{   
    return ('^' != char_i) && ('(' != char_i);
}

static double Addition(double num1, double num2)
{
    return num1 + num2;
}

static double Subtraction(double num1, double num2)
{
    return num2 - num1;
}

static double Multiplication(double num1, double num2)
{
    return num1 * num2;
}

static double Division(double num1, double num2)
{
    return num2 / num1;
}

static int IsDivisionValid(double num1, double num2)
{
    UNUSED(num2);

    return (0 != num1);
}

static double Power(double num1, double num2)
{
    return pow(num2, num1);
}

static int IsPowerValid(double left_num, double right_num)
{
    if (0 == left_num && 0 >= right_num)
    {
        return 0;
    }

    return 1;
}

static int ValidExp(double num1, double num2)
{
    UNUSED(num1);
    UNUSED(num2);

    return 1;
}

static int InvalidExp(double num1, double num2)
{
    UNUSED(num1);
    UNUSED(num2);

    return 0;
}

static status_t FoldDown(char char_i, double num, stack_t *num_stack, stack_t *op_stack)
{
    status_t status = SUCCESS;
    int is_precedence = 1;

    assert(NULL != num_stack);
    assert(NULL != op_stack);

    UNUSED(num);
    while (!StackIsEmpty(op_stack) && (SUCCESS == status) && (IsPrecedence(char_i, num_stack, op_stack)))
    {
         status = Calculate(num_stack, op_stack);
    }

    return status;
}

static status_t FoldParenthesis(char char_i, double num, stack_t *num_stack, stack_t *op_stack)
{
    status_t status = SUCCESS;
    int is_precedence = 1;

    assert(NULL != num_stack);
    assert(NULL != op_stack);
    
    UNUSED(num);

    while (!StackIsEmpty(op_stack) && (SUCCESS == status) && (IsPrecedence(char_i, num_stack, op_stack)))
    {
         status = Calculate(num_stack, op_stack);
    }

    PopParenthesis(op_stack);
    
    return status;
}

static void PopParenthesis(stack_t *op_stack)
{
    assert(NULL != op_stack);

    if (!StackIsEmpty(op_stack) && ('(' == *(char *)StackPeek(op_stack)))
    {
        StackPop(op_stack);
    }
}

static status_t Calculate(stack_t *num_stack, stack_t *op_stack)
{
    double num1 = 0.0;
    double num2 = 0.0;
    double result = 0.0;
    int operator = 0;
    status_t status = SUCCESS;

    assert(NULL != num_stack);
    assert(NULL != op_stack);

    num1 = *(double *)StackPeek(num_stack);
    StackPop(num_stack);
    
    num2 = *(double *)StackPeek(num_stack);
    StackPop(num_stack);
    
    operator = *(char *)StackPeek(op_stack);
    StackPop(op_stack);

    if (!LUT_is_valid[operator](num1, num2))
    {
        status = INVALID_EXP;
    }
    
    result = LUT_Ops[operator](num1, num2);
    StackPush(num_stack, &result);

    return status;
}

static status_t DoNoth(char char_i, double num, stack_t *num_stack, stack_t *op_stack)
{
    UNUSED(char_i);
    UNUSED(num);
    UNUSED(num_stack);
    UNUSED(op_stack);

    return SUCCESS;
}

static status_t Error(char char_i, double num, stack_t *num_stack, stack_t *op_stack)
{
    UNUSED(char_i);
    UNUSED(num);
    UNUSED(num_stack);
    UNUSED(op_stack);

    return INVALID_EXP;
}