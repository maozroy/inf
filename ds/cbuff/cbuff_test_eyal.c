#include <stdlib.h> /* malloc */
#include <stdio.h>
#include "cbuff.h"
#include <string.h> /* strcmp */
#include "my_utils.h"

#define CAPACITY 5

void TestCBuffCreate();
void TestCBuffSpaceLeft();
void TestCBuffWrite();
void TestCBuffRead();
void TestCBuffIsEmptyAndCapacity();

void TestCBuffIsEmptyAndCapacity()
{
	cbuff_t *cbuff = CBuffCreate(CAPACITY);
	char *src = "1234";
	char *dest = (char *)calloc(CAPACITY, sizeof(char));
	
	PRINTTESTRESULTS("CBBuffCapacity", 1, (CAPACITY ==CBuffCapacity(cbuff)));
	PRINTTESTRESULTS("CBuffIsEmpty", 1, (1 ==CBuffIsEmpty(cbuff)));
	CBuffWrite(cbuff, src, CAPACITY - 1);		
	PRINTTESTRESULTS("CBuffIsEmpty", 2, (0 ==CBuffIsEmpty(cbuff)));
	CBuffDestroy(cbuff);
	free(dest);
}

void TestCBuffRead()
{
	cbuff_t *cbuff = CBuffCreate(CAPACITY);
	char *src = "1234";
	char *dest = (char *)calloc(CAPACITY, sizeof(char));


	CBuffWrite(cbuff, src, CAPACITY - 1);		
	
	PRINTTESTRESULTS("CBuffRead", 1, ((CBuffRead(cbuff, dest, 4) == 4)));
	PRINTTESTRESULTS("CBuffRead", 2, ((0 == strcmp(dest, "1234"))));
	
	src = "abcd";
	CBuffWrite(cbuff, src, CAPACITY - 1);
	
	CBuffRead(cbuff, dest, 4);

	PRINTTESTRESULTS("CBuffRead", 3, ((0 == strcmp(dest, "abcd"))));			
/*	*/
	CBuffDestroy(cbuff);
	free(dest);
}

void TestCBuffWrite()
{
	cbuff_t *cbuff = CBuffCreate(CAPACITY);
	char *src = "abcd";
	ssize_t res = 0;
	
	res = CBuffWrite(cbuff, src, CAPACITY - 1);	
	PRINTTESTRESULTS("CBuffWrite", 1, (res == CAPACITY - 1));
	CBuffDestroy(cbuff);
}

void TestCBuffSpaceLeft()
{
	cbuff_t *cbuff = CBuffCreate(CAPACITY);
	
	PRINTTESTRESULTS("CBuffSpaceLeft", 1, (CBuffSpaceLeft(cbuff) == CAPACITY));
	CBuffDestroy(cbuff);
}

void TestCBuffCreate()
{
	cbuff_t *cbuff = CBuffCreate(CAPACITY);
  	PRINTTESTRESULTS("CBuffCreate", 1, (cbuff != NULL));
  	CBuffDestroy(cbuff);
}

int main()
{
	TestCBuffIsEmptyAndCapacity();
	TestCBuffRead();
		TestCBuffWrite();
	TestCBuffSpaceLeft();
	TestCBuffCreate();/**/
	return 0;
}
