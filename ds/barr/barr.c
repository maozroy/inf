/****************************************************************************
*										                                    *
*			                             	 		                        *
*		  	                OL79 - 30/10/19		                            *
*										                                    *	
*                  Worksheet 1 - Data Structures - Intro                    * 
*                               Bits Array                                  *
*                    Reviewer: EyalR                                        *
* 		             Author:   MaozR                                        *
*****************************************************************************/

#include "barr.h"
#define BITS_IN_WORD (sizeof(word) * CHAR_BIT)
# define MSB (0xffffffffffffffff)
# define LSB (1)
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef size_t word;

/*prints bits in one byte*/
char* BArrToString_char(char* buffer ,char arr);
/*mirroring one byte*/
char BArrMirror_char(unsigned char arr);
/*counts bits in byte*/
size_t BArrCountOnChar(char arr);

const unsigned int Mirror_lut [] = {0, 128, 64, 192, 32, 160, 96, 224, 16, 144,
 80, 208, 48, 176, 112, 240, 8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 
216, 56, 184, 120, 248, 4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 
52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 
188, 124, 252, 2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 
114, 242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 
250, 6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246, 
14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254, 1, 
129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 
73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249, 5, 133, 69, 
197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245, 13, 141, 77, 205, 
45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253, 3, 131, 67, 195, 35, 
163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243, 11, 139, 75, 203, 43, 171, 
107, 235, 27, 155, 91, 219, 59, 187, 123, 251, 7, 135, 71, 199, 39, 167, 103, 
231, 23, 151, 87, 215, 55, 183, 119, 247, 15, 143, 79, 207, 47, 175, 111, 239, 
31, 159, 95, 223, 63, 191, 127, 255};

const unsigned int Count_lut[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 
4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3,
3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2,
3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 
6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 
4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 
4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 
4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 
6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};	


bitarray BArrSetAll(bitarray arr)
{	
	return (arr | ~0);
}

bitarray BArrResetAll(bitarray arr)
{
	return (arr & 0);
}

char* BArrToString(char* buffer ,bitarray arr)
{
	size_t i = 0;
	
	assert(buffer);
	
	for (i = 0 ; i < BITS_IN_WORD ; i++)
	{
		*(buffer + BITS_IN_WORD - i - 1) = BArrIsOn(arr, i);
	}
	
	return buffer;
}

bitarray BArrSetBit(bitarray arr, size_t loc, int mod)
{
	size_t replacer = 1;
	
	assert(arr);
	
	replacer <<= loc;
	replacer = (mod == 1) ? replacer : ~replacer;
	arr = ( mod ==1 ) ? arr | replacer : arr & replacer;	
	
	return arr;
}

bitarray BArrSetOn(bitarray arr, size_t loc)
{
	size_t replacer = 1;
	
	assert(arr);
	
	replacer <<= loc;
	arr |= replacer;
	
	return arr;
}

bitarray BArrSetOff(bitarray arr, size_t loc)
{
	size_t replacer = 1;
	
	assert(arr);
	
	replacer <<= loc;
	replacer = ~replacer;
	arr &= replacer;
	
	return arr;	
}

size_t BArrCountOnLUT(bitarray arr)
{
	size_t result = 0;
	int i = 0;

	assert(arr);
	
	for (i = 0 ; i < CHAR_BIT ; i++)
	{
		result += Count_lut[(unsigned char)*( ((char*)&arr) +i) ];
	}

	return result;
}

size_t BArrCountOn(bitarray arr)
{
	size_t mask1 = 0x5555555555555555; /*01010101*/
	size_t mask2 = 0x3333333333333333; /*00110011*/
	size_t mask3 = 0x0F0F0F0F0F0F0F0F; /*00001111*/
	size_t mask4 = 0x00FF00FF00FF00FF; /*0000000011111111*/
	size_t mask5 = 0x0000FFFF0000FFFF; /*00000000000000001111111111111111*/
	size_t mask6 = 0x00000000FFFFFFFF; /*0000000000000000000000000000000011111111111111111111111111111111*/
	
	arr = ( (arr & mask1) + ((arr>>BITS_IN_WORD/64) & mask1) );
	arr = ( (arr & mask2) + ((arr>>BITS_IN_WORD/32) & mask2) );
	arr = ( (arr & mask3) + ((arr>>BITS_IN_WORD/16) & mask3) );
	arr = ( (arr & mask4) + ((arr>>BITS_IN_WORD/8) & mask4) );
	arr = ( (arr & mask5) + ((arr>>BITS_IN_WORD/4) & mask5) );
	arr = ( (arr & mask6) + ((arr>>BITS_IN_WORD/2) & mask6) );

	return arr;
}

size_t BArrCountOnChar(char arr)
{
	size_t mask1 = 0x5555555555555555; /*01010101*/
	size_t mask2 = 0x3333333333333333; /*00110011*/
	size_t mask3 = 0x0F0F0F0F0F0F0F0F; /*00001111*/
	
	arr = ( (arr & mask1) + ((arr>>BITS_IN_WORD/64) & mask1) );
	arr = ( (arr & mask2) + ((arr>>BITS_IN_WORD/32) & mask2) );
	arr = ( (arr & mask3) + ((arr>>BITS_IN_WORD/16) & mask3) );

	return arr;
}

size_t BArrCountOff(bitarray arr)
{
	arr = BITS_IN_WORD - BArrCountOn(arr);

	return arr;
}

int BArrIsOn(bitarray arr, size_t loc)
{
	arr <<= BITS_IN_WORD - 1 - loc;
	arr >>= BITS_IN_WORD - 1;
	
	return arr;
}

int BArrIsOff(bitarray arr, size_t loc)
{
	arr <<= BITS_IN_WORD - 1 - loc;
	arr >>= BITS_IN_WORD - 1;
	arr == 0 ? (arr = 1) : (arr = 0);
	
	return arr;
}

bitarray BArrRotL(bitarray arr, size_t num)
{
	num = num % BITS_IN_WORD;
	arr = (arr<<num ) | (arr>>(BITS_IN_WORD - num) );
	
	return arr;
}

bitarray BArrRotR(bitarray arr, size_t num)
{
	num = num % BITS_IN_WORD;
	arr = (arr>>num ) | (arr<<(BITS_IN_WORD - num) );

	return arr;
}

bitarray BArrFlip(bitarray arr, size_t loc)
{
	int is_off = BArrIsOff(arr, loc);
	
	return arr = BArrSetBit(arr, loc, is_off);
}

bitarray BArrMirror(bitarray arr)
{
	size_t return_value = 0;
	unsigned int i = 0;

	for(i = 0 ; i < BITS_IN_WORD-1 ; i++)
	{
		return_value |= (LSB & arr);
		arr >>=LSB;
		return_value<<=LSB;
	}

	return return_value;
}

bitarray BArrMirrorLUT(bitarray arr)
{
	unsigned int i = 0;
	bitarray result = 0;

	for(i = 0 ; i < CHAR_BIT ; i++)
	{
		result <<= CHAR_BIT;
		result |= Mirror_lut[*(((unsigned char*)&(arr)) + i)];
	}

	return result;
}

char BArrMirror_char(unsigned char arr)
{
	unsigned char return_value = 0;
	unsigned int i = 0;

	for(i = 0 ; i < CHAR_BIT ; i++)
	{
		return_value<<=LSB;
		return_value |= (LSB & arr);
		arr >>=LSB;
	}

	return return_value;
}

char* BArrToString_char(char* buffer ,char arr)
{
	size_t i = 0;
	size_t mask = 1;
	
	assert(buffer);
	
	for(i = 0 ; i < CHAR_BIT ; i++)
	{
		if(arr & mask)
		{
			*(buffer + CHAR_BIT - i - 1) = '1';
		}else
		{
			*(buffer + CHAR_BIT - i - 1) = '0';
		}
		mask <<= 1;
	}
	
	return buffer;
}
