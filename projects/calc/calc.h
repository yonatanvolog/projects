/****************************************************************************
*                                                                           *
*                               OL79 - 17/12/19                             *
*                                                                           *	
*                                Calculator                                 *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_CALC_H
#define ILRD_CALC_H

typedef enum status
{
	SUCCESS,
	DIVISION_BY_ZERO,
	INVALID_EXP,
	ALLOC_FAIL
} status_t;

/* Calc receives two arguments and returns a status type
 * @exp: a pointer of type const char *
 * @res: a pointer of type double *
 * @exp and @res must be valid pointers, otherwise, behavior is undefined
 * On success the function returns SUCCESS
 * On failure the function returns DIVISION_BY_ZERO if that's the case or
 * INVALID_EXP if otherwise.
 * Check status before using @res
 */
status_t Calc(const char *exp, double *res);

#endif
