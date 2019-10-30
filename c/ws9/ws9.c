/* WS9 - Word Boundary optimization */
/* Author - Maoz Roytman */
/* Reviewer - Chen Rubin */
/* Date - 29/10/19 */

#include <stdio.h> /*prints*/
#include <string.h> /* memset */
#include <stdlib.h> /*mallocs and atoi*/
#include <math.h> /* POW */
#include <assert.h> /* assert */
#include "ws9.h"

#define ASCI_NUM 255
#define WORD sizeof(size_t)

char LUTable[36] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

void *MyMemcpy(void *str1, void *str2, size_t n)
{	
	size_t *st1_p = str1;
	size_t *st2_p = str2;
	char *ch1_p = str1;
	char *ch2_p = str2;
	unsigned int num_of_ending_bytes = 0;
	unsigned int num_of_trailing_bytes = WORD - ( (*(size_t*) & st1_p) % WORD);
	unsigned int num_of_words = 0;
	
	assert(str1);
	assert(str2);
	
	if (WORD == num_of_trailing_bytes)
	{
		num_of_trailing_bytes = 0;
	}
	
	num_of_words = (n - num_of_trailing_bytes) / WORD;
	ByteMemcpy(ch1_p, ch2_p, num_of_trailing_bytes); 
	ch1_p = ch1_p + num_of_trailing_bytes;
	ch2_p = ch2_p + num_of_trailing_bytes;
	st1_p =(size_t*) ch1_p;
	st2_p =(size_t*) ch2_p;
	
	WordMemcpy(st1_p, st2_p, num_of_words); 
	st1_p = st1_p + num_of_words;
	st2_p = st2_p + num_of_words;
	ch1_p = (char*)st1_p;
	ch2_p = (char*)st2_p;
	
	num_of_ending_bytes = n - num_of_words*(WORD) - num_of_trailing_bytes;
	ByteMemcpy(ch1_p, ch2_p, num_of_ending_bytes);
	
	return str1;
}

void *MyMemmove (void *dst_p, void *src_p, unsigned int n)
{
	size_t dst_num = (*(size_t*) & dst_p) + n;
	size_t src_num = (*(size_t*) & src_p) + n;

	char *src_ch_p = (char*)src_p + n-1;
	char *dst_ch_p = (char*)dst_p + n-1;
	size_t *src_st_p = NULL;
	size_t *dst_st_p = NULL;
	
	unsigned int num_of_trailing_bytes = WORD - (dst_num % WORD);
	unsigned int num_of_words = 0;
	unsigned int num_of_ending_bytes = 0;
	unsigned int i = 0;
	
	assert(dst_p);
	assert(src_p);
	
	if (WORD == num_of_trailing_bytes)
	{
		num_of_trailing_bytes = 0;
	}
	
	if (num_of_trailing_bytes > n)
	{
		num_of_trailing_bytes = n;
	}
	
	num_of_words = (n - num_of_trailing_bytes) / WORD;
	num_of_ending_bytes = n - num_of_words*WORD - num_of_trailing_bytes;
	
	if ( !(( dst_num >  src_num ) && ( dst_num <= src_num+n)) )
	{
		MyMemcpy(dst_p, src_p, n);
		
		return dst_p;
	}
	for (i = 0 ; i < num_of_trailing_bytes ; ++i)
	{
		*(dst_ch_p - i) = *(src_ch_p - i);
	}
	
	dst_ch_p = dst_ch_p - num_of_trailing_bytes + 1;
	src_ch_p = src_ch_p - num_of_trailing_bytes + 1;
	src_st_p = (size_t*) src_ch_p - 1;
	dst_st_p = (size_t*) dst_ch_p - 1;
	
	for (i = 0 ; i < num_of_words; ++i)
	{
		*(dst_st_p-i) = *(src_st_p-i);
	}
	
	dst_st_p = dst_st_p - num_of_words + 1;
	src_st_p = src_st_p - num_of_words + 1;
	dst_ch_p = (char*)dst_st_p - 1;
	src_ch_p = (char*)src_st_p - 1;
	
	for (i = 0 ; i < num_of_ending_bytes ; ++i)
	{
		*(dst_ch_p - i) = *(src_ch_p - i);
	}
	
	return dst_p;
}

void *MyMemset (void *str, int c, size_t n)
{
	unsigned int i = 0;
	size_t *st_p = str;
	char *ch_p = str;
	size_t insert_to_string = 0;
	unsigned int num_of_ending_bytes = 0;
	unsigned int num_of_trailing_bytes = WORD - ( (*(size_t*) & st_p) % WORD);
	unsigned int num_of_words = 0;
	
	assert(str);
	
	if (WORD == num_of_trailing_bytes)
	{
		num_of_trailing_bytes = 0;
	}
	
	if (num_of_trailing_bytes > n)
	{
		num_of_trailing_bytes = n;
	}
	
	/*check if n is too large*/
	if (strlen(ch_p) < n)
	{
		n = strlen(ch_p);
	}
	
	num_of_words = (n - num_of_trailing_bytes) / WORD;
	/*define block of c*/
	for (; i < WORD ; ++i)
	{

		insert_to_string <<= WORD;
		insert_to_string |= c;
	}
	
	ByteMemset(ch_p, c, num_of_trailing_bytes); /*start tail loop*/
	ch_p = ch_p+num_of_trailing_bytes;
	st_p =(size_t*) ch_p;
	
	for (i = 0; i < num_of_words ; ++i)
	{
		*(st_p) = insert_to_string;
		++st_p;
	}
	
	ch_p = (char*)st_p;
	num_of_ending_bytes = n - num_of_words*(WORD) - num_of_trailing_bytes;
	ByteMemset(ch_p, c, num_of_ending_bytes); /*end tail loop*/
	
	return str;
}

void ByteMemset(char *ptr, int c, size_t n)
{
	unsigned int i = 0;
	
	assert(ptr);
	
	for (i = 0 ; i < n ; ++i)
	{
		*ptr = c;
		++ptr;
	}
}

void ByteMemcpy(char *ptr1, const char *ptr2, size_t n)
{
	unsigned int i = 0;
	
	assert(ptr1);
	assert(ptr2);
	
	for (i = 0 ; i < n ; ++i)
	{
		*ptr1 = *ptr2;
		++ptr1;
		++ptr2;
	}
}

void WordMemcpy(size_t *ptr1, const size_t *ptr2, size_t n)
{
	unsigned int i = 0;
	
	assert(ptr1);
	assert(ptr2);
	
	for (i = 0 ; i < n ; ++i)
	{
		*ptr1 = *ptr2;
		++ptr1;
		++ptr2;
	}
}

char *MyItoa(int num, char* str, int base)
{
	char *temp_str = malloc(sizeof(char)*999);
	char *temp_base = temp_str;
	int minus_flag = 0;
	
	if(NULL == temp_str)
	{
		printf("\nno space available for malloc!\n");
		return;
	}
	
	assert(str);

	temp_str += 99;
	if (num < 0)
	{
		num *= -1;
		minus_flag = 1;
	}
	
	*temp_str = '\0';
	--temp_str;

	while (num>0)
	{
		*temp_str = LUTable[num % base];
		--temp_str;
		num /= base;
	}
	
	if (1 == minus_flag)
	{
		*temp_str = '-';
		--temp_str;
	}
	++temp_str;
	strcpy(str, temp_str);
	free(temp_base);
	
	return str;
}

int MyAtoi(char* str, size_t base)
{
	size_t len = strlen(str);
	int result = 0;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int neg_flag = 0;
		
	assert(str);

	if ('-' == *(str))
	{
		++str;
		--len;
		neg_flag = 1;
	}

	for (i = 0; i < len; ++i)
	{
		for (j = 0 ; j < base ; ++j)
		{
			if(*(str + i) == LUTable[j])
			{
				break;
			}
		}
		result += j * pow(base, (len-i - 1));
	}
	
	if (neg_flag)
	{
		result *= -1;
	}
	
	return result;
}

void CheckThreeArrays (char* arr1, char* arr2, char* arr3)
{
	unsigned int i = 0;
	unsigned int cmp_arr[ASCI_NUM];
	size_t len1 = strlen(arr1);
	size_t len2 = strlen(arr2);
	size_t len3 = strlen(arr3);
		
	assert(arr1);
	assert(arr2);
	assert(arr3);

	for (i = 0 ; i < ASCI_NUM ; ++i)
	{
		cmp_arr[i] = 0;
	}
	
	for (i = 0 ; (i < len1) ; ++i)
	{
		cmp_arr[(int)*(arr1 + i)] = 1 ;
	}
	
	for (i = 0 ; (i < len2) ; ++i)
	{
		if (1 == cmp_arr[(int)*(arr2 + i)])
		{
			cmp_arr[(int)*(arr2 + i)] += 1; 
		}
	}
	
	for (i = 0 ; (i < len3) ; ++i)
	{
		cmp_arr[(int)*(arr3 + i)] = 0 ; 
	}
	
	for (i = 0 ; i < ASCI_NUM ; ++i)
	{
		if (cmp_arr[i] >= 2)
		{
			printf("%c\n", (char)i);
		}
		
	}
}

void IsLittleEndian()
{
	unsigned int i = 1;
	char *c = (char*)&i;

	if (*c)
	{
		printf("\nLil Ind\n");
	}else
	{
		printf("\nBig Ind\n");
	}
}
