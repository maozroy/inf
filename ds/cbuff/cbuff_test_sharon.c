#include <stdio.h> /* printf */
#include <string.h>
#include <stdlib.h> 

#include "cbuff.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 

struct CB
{
size_t capacity;
char *r_ptr;
char *w_ptr;
char begin[1];
};

int main()
{
	size_t space_left = 0;
	size_t capacity = 0;
	const char *src1 = "hello";
	const char *src2 = "123456789123456789";
	const char *src3 ="hell123456";
	const char *src4 = "1234567";
	ssize_t result = 0;
	char *dest=(char*)calloc(30, sizeof(char)); 

	cbuff_t *cbuff = CBuffCreate(10);
	printf("CBuffCreate\n");
	TEST2(cbuff);
	printf("----------------\n");
	printf("CBuffCapacity\n");
	capacity = CBuffCapacity(cbuff);
	TEST1(capacity, 10);
	printf("----------------\n");
	printf("CBuffIsEmpty\n");
	TEST1(CBuffIsEmpty(cbuff), 1);
	printf("----------------\n");
	printf("CBuffSpaceLeft\n");
	space_left = CBuffSpaceLeft(cbuff);
	TEST1(space_left, 10);
	printf("----------------\n");
	printf("CBuffWrite\n");
	result = CBuffWrite(cbuff, src1, 4);
	TEST1(CBuffSpaceLeft(cbuff), 6);
	TEST1(result, 4);
	TEST1(memcmp(cbuff->begin, src1, 4), 0);
	result = CBuffWrite(cbuff, src2, 7);
	TEST1(CBuffSpaceLeft(cbuff), 0);
	TEST1(result, 6);
	TEST1(memcmp(cbuff->begin, src3, 10), 0);
	printf("----------------\n");
	printf("CBuffRead\n");
	result = CBuffRead(cbuff, dest, 4);
	TEST1(memcmp(dest, "hell", 4), 0);
	TEST1(CBuffSpaceLeft(cbuff), 4);
	TEST1(result, 4);
	TEST1(memcmp(cbuff->r_ptr, src4, 6), 0);
	TEST1(CBuffSpaceLeft(cbuff), 4);
	TEST1(CBuffIsEmpty(cbuff), 0);
	result = CBuffWrite(cbuff, src2, 14);
	TEST1(result, 4);
	TEST1(memcmp(cbuff->r_ptr, "1234561", 7), 0);
	TEST1(memcmp(cbuff->begin, "234l1234561", 11), 0);
	result = CBuffRead(cbuff, dest, 13);
	TEST1(result, 10);
	TEST1(CBuffIsEmpty(cbuff), 1);
	result = CBuffRead(cbuff, dest, 13);
	TEST1(result, 0);
	CBuffDestroy(cbuff);
	free(dest);
	
	return 0;
}
