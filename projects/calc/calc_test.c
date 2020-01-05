/**********************************************************************
 *	This code is representing my tests of calc		                  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 22.12.19													  *
 **********************************************************************/
#include <stdio.h>	/*printf*/

#include "calc.h"
#include "../../yoni_utils.h"

int PlusMinusTest();
int MultiplicationTest();
int DivisionTest();
int MultiplicationDivisionTest();
int PlusMinusMultiplicationDivisionTest();
int DecimalsTest();
int InvalidExpressionTest();
int ParenthesisTest();
int PowerTest();

int main()
{
	RUN_SEQUENCE("PlusMinusTest", PlusMinusTest());
	RUN_SEQUENCE("MultiplicationTest", MultiplicationTest());
	RUN_SEQUENCE("DivisionTest", DivisionTest());
	RUN_SEQUENCE("MultiplicationDivisionTest", MultiplicationDivisionTest());
	RUN_SEQUENCE("PlusMinusMultiplicationDivisionTest", PlusMinusMultiplicationDivisionTest());	
	RUN_SEQUENCE("DecimalsTest", DecimalsTest());	
	RUN_SEQUENCE("InvalidExpressionTest", InvalidExpressionTest());
	RUN_SEQUENCE("ParenthesisTest", ParenthesisTest());
	RUN_SEQUENCE("PowerTest", PowerTest());

	return 0;
}

int PlusMinusTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "1";
	char exp2[] = "1+2";
	char exp3[] = "1+21+3";
	char exp4[] = "1+232-453-4+5645-10-7001";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: 1\t\t", 1 == res);
	RUN_TEST("status: 1\t", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: 1+2\t", 3 == res);
	RUN_TEST("status: 1+2\t", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: 1+21+3\t", 25 == res);
	RUN_TEST("status: 1+2+3\t", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("res: 1+232-453-4+5645-10-7001\t", -1590 == res);
	RUN_TEST("status: 1+232-453-4+5645-10-7001", SUCCESS == return_status);

	return 0;
}

int MultiplicationTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "1*1";
	char exp2[] = "1*2";
	char exp3[] = "1*21*3";
	char exp4[] = "1*243*534*96*7";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: 1*1\t", 1 == res);
	RUN_TEST("status: 1*1\t", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: 1*2\t", 2 == res);
	RUN_TEST("status: 1*2\t", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: 1*21*3\t", 63 == res);
	RUN_TEST("status: 1*21*3\t", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("res: 1*243*534*96*7", 87200064 == res);
	RUN_TEST("status: 1*243*534*96*7", SUCCESS == return_status);

	return 0;
}

int DivisionTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "1/1";
	char exp2[] = "1/2";
	char exp3[] = "21/3/1";
	char exp4[] = "2250/4/100/125";
	char exp5[] = "1/0";
	char exp6[] = "2250/4/0/125";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: 1/1\t", 1 == res);
	RUN_TEST("status: 1*1\t", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: 1/2\t", 0.5 == res);
	RUN_TEST("status: 1*2\t", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: 21/3/1\t", 7 == res);
	RUN_TEST("status: 21/3/1\t", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("res: 2250/4/100/125", 0.045 == res);
	RUN_TEST("status: 2250/4/100/125", SUCCESS == return_status);

	return_status = Calc(exp5, &res);
	RUN_TEST("status: 1/0\t", DIVISION_BY_ZERO == return_status);

	return_status = Calc(exp6, &res);
	RUN_TEST("status: 2250/4/0/125", DIVISION_BY_ZERO == return_status);

	return 0;
}

int MultiplicationDivisionTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "6/2*3";
	char exp2[] = "1*2*3/6";
	char exp3[] = "21*5/4*65/5";
	char exp4[] = "21*5/0*65/5";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: 6/2*3\t", 9 == res);
	RUN_TEST("status: 6/2*3\t", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: 1*2*3/6\t", 1 == res);
	RUN_TEST("status: 1*2*3/6\t", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: 21*5/4*65/5", 341.25 == res);
	RUN_TEST("status: 21*5/4*65/5", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("status: 21*5/0*65/5", DIVISION_BY_ZERO == return_status);

	return 0;
}

int PlusMinusMultiplicationDivisionTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "1+2-4*5/2";
	char exp2[] = "1+2*6+2/4";
	char exp3[] = "21*5/4*65/5";
	char exp4[] = "85/5+7-88*55/10/5/5*3+1/20";
	char exp5[] = "85/5+7-88*55/10/0/5*3+1/20";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: 1+2-4*5/2\t", -7 == res);
	RUN_TEST("status: 1+2-4*5/2", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: 1+2*6+2/4\t", 13.5 == res);
	RUN_TEST("status: 1+2*6+2/4", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: 21*5/4*65/5", 341.25 == res);
	RUN_TEST("status: 21*5/4*65/5", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("res: 85/5+7-88*55/10/5/5*3+1/20", -34.03 == res);
	RUN_TEST("status: 85/5+7-88*55/10/5/5*3+1/20", SUCCESS == return_status);

	return_status = Calc(exp5, &res);
	RUN_TEST("status: 85/5+7-88*55/10/0/5*3+1/20", DIVISION_BY_ZERO == return_status);

	return 0;
}

int DecimalsTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "1.1+1";
	char exp2[] = "1.1+1*2.5";
	char exp3[] = "1.1+1*2.4-5/2.5-20";
	char exp4[] = "0.1+1*0.4-5/0.5-20+0.1";
	char exp5[] = "1.1+1*2.4+5/2.5/0";
	char exp6[] = ".5+2";
	char exp7[] = ".5*4+.44";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: 1.1+1\t", 2.1 == res);
	RUN_TEST("status: 1.1+1\t", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: 1.1+1*2.5\t", 3.6 == res);
	RUN_TEST("status: 1.1+1*2.5\t", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: 1.1+1*2.4-5/2.5-20", -18.5 == res);
	RUN_TEST("status: 1.1+1*2.4-5/2.5-20", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("res: 0.1+1*0.4-5/0.5-20+0.1", -29.4 == res);
	RUN_TEST("status: 0.1+1*0.4-5/0.5-20+0.1", SUCCESS == return_status);

	return_status = Calc(exp5, &res);
	RUN_TEST("status: 1.1+1*2.4+5/2.5/0", DIVISION_BY_ZERO == return_status);

	return_status = Calc(exp6, &res);
	RUN_TEST("res: .5+2\t", 2.5 == res);
	RUN_TEST("status: .5+2\t", SUCCESS == return_status);

	return_status = Calc(exp7, &res);
	RUN_TEST("res: .5*4+.44\t", 2.44 == res);
	RUN_TEST("status: .5*4+.44", SUCCESS == return_status);

	return 0;
}

int InvalidExpressionTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "+";
	char exp2[] = "1+2-";
	char exp3[] = "1*2+-";
	char exp4[] = "((1)+2))";
	char exp5[] = "1+2+*3";
	char exp6[] = "1/2*7^(/20)";
	char exp7[] = "(*1)";
	char exp8[] = "*1*";
	char exp9[] = "0^0";
	
	
	return_status = Calc(exp1, &res);
	RUN_TEST("status: +\t", INVALID_EXP == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("status: 1+2-\t", INVALID_EXP == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("status: 1*2+-\t", INVALID_EXP == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("status: ((1)+2))", INVALID_EXP == return_status);

	return_status = Calc(exp5, &res);
	RUN_TEST("status: 1+2+*3\t", INVALID_EXP == return_status);

	return_status = Calc(exp6, &res);
	RUN_TEST("status: 1/2*7^(/20)", INVALID_EXP == return_status);

	return_status = Calc(exp7, &res);
	RUN_TEST("status: (*1)\t", INVALID_EXP == return_status);

	return_status = Calc(exp8, &res);
	RUN_TEST("status: *1*\t", INVALID_EXP == return_status);

	return_status = Calc(exp9, &res);
	RUN_TEST("status: 0^0\t", INVALID_EXP == return_status);

	return 0;
}

int ParenthesisTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "(1)";
	char exp2[] = "(1+2)";
	char exp3[] = "(-1)+(-5)";
	char exp4[] = "(1*20)/4+(2/(-2))";
	char exp5[] = "((((1+5)/2)/3)+20+1)*2";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: (1)\t", 1 == res);
	RUN_TEST("status: (1)\t", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: (1+2)\t", 3 == res);
	RUN_TEST("status: (1+2)\t", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: (-1)+(-5)\t", (-6) == res);
	RUN_TEST("status: (-1)+(-5)\t", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("res: (1*20)/4+(2/(-2))\t", 4 == res);
	RUN_TEST("status: (1*20)/4+(2/(-2))", SUCCESS == return_status);

	return_status = Calc(exp5, &res);
	RUN_TEST("res: ((((1+5)/2)/3)+20+1)*2", 44 == res);
	RUN_TEST("status: ((((1+5)/2)/3)+20+1)*2", SUCCESS == return_status);

	return 0;
}

int PowerTest()
{
	status_t return_status = SUCCESS;
	double res = 0;
	char exp1[] = "2^3";
	char exp2[] = "(2+4)^3";
	char exp3[] = "(1+2/2)^(-3)";
	char exp4[] = "(2*2*2)^((-2)/2+3)";
	char exp5[] = "(1+2/4)^((-5)/5)^2";
	
	return_status = Calc(exp1, &res);
	RUN_TEST("res: 2^3\t", 8 == res);
	RUN_TEST("status: 2^3\t", SUCCESS == return_status);
	
	return_status = Calc(exp2, &res);
	RUN_TEST("res: (2+4)^3\t", 216 == res);
	RUN_TEST("status: (2+4)^3\t", SUCCESS == return_status);

	return_status = Calc(exp3, &res);
	RUN_TEST("res: (1+2/2)^(-3)\t", 0.125 == res);
	RUN_TEST("status: (1+2/2)^(-3)\t", SUCCESS == return_status);

	return_status = Calc(exp4, &res);
	RUN_TEST("res: (2*2*2)^((-2)/2+3)\t", 64 == res);
	RUN_TEST("status: (2*2*2)^((-2)/2+3)", SUCCESS == return_status);

	return_status = Calc(exp5, &res);
	RUN_TEST("res: (1+2/4)^((-5)/5)^2\t", 1.5 == res);
	RUN_TEST("status: (1+2/4)^((-5)/5)^2", SUCCESS == return_status);

	return 0;
}
