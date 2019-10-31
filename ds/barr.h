/****************************************************************************
*										                                    *
*			                             	 		                        *
*		  	                OL79 - 28/10/19		                            *
*										                                    *	
*                  Worksheet 1 - Data Structures - Intro                    * 
*                               Bits Array                                  *
*                    Reviewer:                                              *
* 		                                                                    *
*****************************************************************************/
#ifndef BARR_H
#define BARR_H
typedef size_t bitarray;
/*loc argument can get 0-63 as per the bitarray*/

/* set all bits to 1 */
bitarray BArrSetAll(bitarray arr);
/* set all bits to 0 */
bitarray BArrResetAll(bitarray arr);
/* set loc bit to mod. mod can get only 0 or 1 */
bitarray BArrSetBit(bitarray arr, size_t loc, int mod);
/* set loc bit to 1 */
bitarray BArrSetOn(bitarray arr, size_t loc);
/* set loc bit to 0 */
bitarray BArrSetOff(bitarray arr, size_t loc);
/* check if loc bit is 1 */
int BArrIsOn(bitarray arr, size_t loc);
/* check if loc bit is 0 */
int BArrIsOff(bitarray arr, size_t loc);
/* counts on bits */
size_t BArrCountOn(bitarray arr);
/* counts off bits */
size_t BArrCountOff(bitarray arr);
/* rotate entire array right accoriding to num */
bitarray BArrRotR(bitarray arr, size_t num);
/* rotate entire array left accoriding to num */
bitarray BArrRotL(bitarray arr, size_t num);
/*mirrors the array*/
bitarray BArrMirror(bitarray arr);
/*copies the array to buffer*/
char* BArrToString(char* buffer ,bitarray arr);
/*flips specific loc bit*/
bitarray BArrFlip(bitarray arr, size_t loc);

size_t BArrCountOnLUT(bitarray arr);
bitarray BArrMirrorLUT(bitarray arr);

#endif
