/**********************************************************************
 *	This code is representing my implemenation of calc RC2            *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Finkelshtein	 	  								  *
 *	Date: 22.12.19													  *
 **********************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */ 
#include <string.h> /* strlen */
#include <stddef.h> /* size_t */
#include <math.h> 	/* pow */

#include "stack.h"
#include "calc.h"
#include "yoni_utils.h"

#define NUM_OF_CHARS 256
#define LARGEST_ASCII_OF_OP ('/')
#define NUM_OF_STATES 3

typedef enum state_e
{
	WAIT_FOR_NUM_STATE,
	WAIT_FOR_OP_STATE
}state_e;

typedef status_t (*HandleFunc_t)(stack_t *num_stack, stack_t *op_stack,
												void *data, status_t status);
typedef double (*OpFunc_t)(double num1, double num2);

typedef struct state_s
{
	HandleFunc_t handle_func;
	state_e next_state;
}state_s;

static struct state_s transition_LUT[NUM_OF_STATES][NUM_OF_CHARS];
static OpFunc_t op_LUT[LARGEST_ASCII_OF_OP + 1];
size_t precedence_LUT[LARGEST_ASCII_OF_OP + 1];

static status_t HandleNumIMP(stack_t *num_stack, stack_t *op_stack,
												 void *data, status_t status);
static status_t HandleOpIMP(stack_t *num_stack, stack_t *op_stack,
												 void *data, status_t status);
static status_t HandleErrIMP(stack_t *num_stack, stack_t *op_stack,
												 void *data, status_t status);
static status_t HandleOpOpenBracketIMP(stack_t *num_stack, stack_t *op_stack,
												 void *data, status_t status);
static status_t HandleOpCloseBracketIMP(stack_t *num_stack, stack_t *op_stack,
												 void *data, status_t status);
static status_t ValidateParenthesesIMP(char *exp, status_t status);

static double AdditionIMP(double num1, double num2);
static double SubtractionIMP(double num1, double num2);
static double MultiplyIMP(double num1, double num2);
static double DivisionIMP(double num1, double num2);
static double PowerIMP(double num1, double num2);
static void InitLUTSIMP(void);

static int IsNewPrecidenceLowerOrEqIMP(stack_t *op_stack, char new_operator);
static status_t SingleCalculationIMP(stack_t *num_stack, stack_t *op_stack,
														  status_t status);
static status_t CalculateLeftoversIMP(stack_t *num_stack, stack_t *op_stack,
								   state_e current_state, status_t status);
static double GetFinalResultIMP(stack_t *num_stack);

static void InitLUTSIMP(void)
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; i <= NUM_OF_CHARS; ++i)
	{
		for (j = 0; j < NUM_OF_STATES; ++j)
		{
			transition_LUT[j][i].handle_func = HandleErrIMP;
		}
	}

	for (i = '0'; i <= '9'; ++i)
	{
		transition_LUT[WAIT_FOR_NUM_STATE][i].handle_func = HandleNumIMP;
		transition_LUT[WAIT_FOR_NUM_STATE][i].next_state = WAIT_FOR_OP_STATE;
	}
	transition_LUT[WAIT_FOR_NUM_STATE]['.'].handle_func = HandleNumIMP;
	transition_LUT[WAIT_FOR_NUM_STATE]['.'].next_state = WAIT_FOR_OP_STATE;

	transition_LUT[WAIT_FOR_NUM_STATE]['('].handle_func = HandleOpOpenBracketIMP;
	transition_LUT[WAIT_FOR_NUM_STATE]['+'].handle_func = HandleNumIMP;
	transition_LUT[WAIT_FOR_NUM_STATE]['-'].handle_func = HandleNumIMP;

	transition_LUT[WAIT_FOR_NUM_STATE]['('].next_state = WAIT_FOR_NUM_STATE;
	transition_LUT[WAIT_FOR_NUM_STATE]['+'].next_state = WAIT_FOR_OP_STATE;
	transition_LUT[WAIT_FOR_NUM_STATE]['-'].next_state = WAIT_FOR_OP_STATE;

	transition_LUT[WAIT_FOR_OP_STATE][')'].handle_func = HandleOpCloseBracketIMP;
	transition_LUT[WAIT_FOR_OP_STATE]['+'].handle_func = HandleOpIMP;
	transition_LUT[WAIT_FOR_OP_STATE]['-'].handle_func = HandleOpIMP;
	transition_LUT[WAIT_FOR_OP_STATE]['*'].handle_func = HandleOpIMP;
	transition_LUT[WAIT_FOR_OP_STATE]['/'].handle_func = HandleOpIMP;
	transition_LUT[WAIT_FOR_OP_STATE]['^'].handle_func = HandleOpIMP;

	transition_LUT[WAIT_FOR_OP_STATE][')'].next_state = WAIT_FOR_OP_STATE;
	transition_LUT[WAIT_FOR_OP_STATE]['+'].next_state = WAIT_FOR_NUM_STATE;
	transition_LUT[WAIT_FOR_OP_STATE]['-'].next_state = WAIT_FOR_NUM_STATE;
	transition_LUT[WAIT_FOR_OP_STATE]['*'].next_state = WAIT_FOR_NUM_STATE;
	transition_LUT[WAIT_FOR_OP_STATE]['/'].next_state = WAIT_FOR_NUM_STATE;
	transition_LUT[WAIT_FOR_OP_STATE]['^'].next_state = WAIT_FOR_NUM_STATE;

	op_LUT['+'] = AdditionIMP;
	op_LUT['-'] = SubtractionIMP;
	op_LUT['*'] = MultiplyIMP;	
	op_LUT['/'] = DivisionIMP;
	op_LUT['^'] = PowerIMP;

	precedence_LUT['('] = 0;
	precedence_LUT[')'] = 0;
	precedence_LUT['+'] = 1;
	precedence_LUT['-'] = 1;
	precedence_LUT['*'] = 2;
	precedence_LUT['/'] = 2;
	precedence_LUT['^'] = 3;
}

static status_t HandleNumIMP(stack_t *num_stack, stack_t *op_stack,
									 void *data, status_t status)
{
	UNUSE(op_stack);
	StackPush(num_stack, data);

	return status;
}

static status_t HandleOpIMP(stack_t *num_stack, stack_t *op_stack,
									 void *data, status_t status)
{
	char new_operator = *(char *)data;

	while ((!StackIsEmpty(op_stack)) &&
			IsNewPrecidenceLowerOrEqIMP(op_stack, new_operator) &&
			((int)new_operator != '^'))
	{
		SingleCalculationIMP(num_stack, op_stack, status);
	}

	StackPush(op_stack, data);

	return status;
}

static status_t HandleErrIMP(stack_t *num_stack, stack_t *op_stack,
									 void *data, status_t status)
{
	UNUSE(num_stack);
	UNUSE(op_stack);
	UNUSE(data);
	UNUSE(status);

	return INVALID_EXP;
}

static status_t HandleOpOpenBracketIMP(stack_t *num_stack, stack_t *op_stack,
											 void *data, status_t status)
{
	char ch = '(';
	UNUSE(data);
	UNUSE(num_stack);

	StackPush(op_stack, &ch);

	return status;
}

static status_t HandleOpCloseBracketIMP(stack_t *num_stack, stack_t *op_stack, 
											  void *data, status_t status)
{
	UNUSE(data);
	while ((!StackIsEmpty(op_stack)) && 
		(*(char *)StackPeek(op_stack) != '('))
	{
		SingleCalculationIMP(num_stack, op_stack, status);
	}

	StackPop(op_stack);

	return status;
}

static status_t ValidateParenthesesIMP(char *exp, status_t status)
{
	size_t open_counter = 0;
    size_t close_counter = 0;
    char *str_runner = exp;

    while ('\0' != *str_runner)
    {
        if ('(' == *str_runner)
        {
            ++open_counter;
        }
        else if (')' == *str_runner)
        {
            ++close_counter;
        }
        ++str_runner;
    }

    if (open_counter != close_counter)
    {
        status = INVALID_EXP;
    }

    return status;
}

static double AdditionIMP(double num1, double num2)
{
	return ((num2) + (num1));
}

static double SubtractionIMP(double num1, double num2)
{
	return ((num2) - (num1));
}

static double MultiplyIMP(double num1, double num2)
{
	return ((num2) * (num1));
}

static double DivisionIMP(double num1, double num2)
{
	return ((num2) / (num1));
}

static double PowerIMP(double num1, double num2)
{
	return pow((num2), (num1));
}

static int IsNewPrecidenceLowerOrEqIMP(stack_t *op_stack, char new_operator)
{
	return (precedence_LUT[(int)*(char *)StackPeek(op_stack)] >= 
			precedence_LUT[(int)new_operator]);
}

static double GetFinalResultIMP(stack_t *num_stack)
{
	return (*((double *)StackPeek(num_stack)));
}

static status_t SingleCalculationIMP(stack_t *num_stack, stack_t *op_stack,
														 status_t status)
{
	double num1 = 0;
	double num2 = 0;
	char operator = 0;
	double result = 0;
	
	num1 = *(double *)StackPeek(num_stack);
	StackPop(num_stack);

	num2 = *(double *)StackPeek(num_stack);
	StackPop(num_stack);

	operator = *(char *)StackPeek(op_stack);
	StackPop(op_stack);

	result = op_LUT[(int)operator](num1, num2);

	if (isinf(result))
	{
		status = DIVISION_BY_ZERO;
	}

	if((0 == num1) && ('^' == operator) && (0 == num2))
	{
		status = INVALID_EXP;
	}

	StackPush(num_stack, &result);

	return status;
}

static status_t CalculateLeftoversIMP(stack_t *num_stack, stack_t *op_stack,
								  state_e current_state, status_t status)
{
	UNUSE(current_state);

	while (!StackIsEmpty(op_stack) && (status == SUCCESS))
	{
		status = SingleCalculationIMP(num_stack, op_stack, status);
	}

	return status;
}

status_t Calc(const char *exp, double *res)
{
	size_t string_len = 0;
	char *str_runner = NULL;
	char first_char = '\0';
	double num = 0;
	state_e current_state = WAIT_FOR_NUM_STATE;
	status_t status = SUCCESS;
	stack_t *num_stack = NULL;
	stack_t *op_stack = NULL;
	
	assert(exp);
	assert(res);

	string_len = strlen(exp);

	num_stack = StackCreate(string_len, sizeof(double));
	if (NULL == num_stack)
	{
		return ALLOC_FAIL;
	}

	op_stack = StackCreate(string_len, sizeof(char));
	if (NULL == num_stack)
	{
		StackDestroy(num_stack);

		return ALLOC_FAIL;
	}

	InitLUTSIMP();
	str_runner = (char *)exp;
	status = ValidateParenthesesIMP(str_runner, status);

	while (('\0' != *str_runner) && (SUCCESS == status))
	{
		first_char = *str_runner;
		
		/*Parses and handles number*/
		if (WAIT_FOR_NUM_STATE == current_state)
		{
			num = strtod(str_runner, &str_runner);
			status = transition_LUT[current_state][(int)first_char].handle_func
										(num_stack, op_stack, &num, status);
			if ('(' == first_char)
			{
				++str_runner;
			}
		}

		/*Parses and handles operator*/
		else if (WAIT_FOR_OP_STATE == current_state)
		{

			status = transition_LUT[current_state][(int)first_char].handle_func
									(num_stack, op_stack, &first_char, status);
			++str_runner;
		}

		current_state = transition_LUT[current_state][(int)first_char].next_state;
	}

	if (WAIT_FOR_NUM_STATE == current_state)
	{
		status = INVALID_EXP;
	}

	status = CalculateLeftoversIMP(num_stack, op_stack, current_state, status);

	if (!StackIsEmpty(num_stack))
	{
		*res = GetFinalResultIMP(num_stack);
	}

	StackDestroy(num_stack);
	StackDestroy(op_stack);

	return status;
}
