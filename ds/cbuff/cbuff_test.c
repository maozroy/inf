#include "cbuff.h"
#include <stdio.h>
#define CAPACITEST 10

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n",\
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m"))\

struct CB{
	size_t capacity;
	char *r_ptr;
	char *w_ptr;
	char begin[1];
};

void TestCBuffCreate();
void TestCBuffDestroy();
void TestCBuffRead();
void TestCBuffWrite();
void TestCBBuffSpaceLeft();
void TestCBuffIsEmpty();
void TestCBBuffCapacity();



int main()
{
TestCBuffCreate();
TestCBuffWrite();

return 0;
}

void TestCBuffCreate()
{

	cbuff_t *my_buff = CBuffCreate(CAPACITEST);
	my_buff->w_ptr+=9;
	my_buff->r_ptr+=0;
	CBuffDestroy(my_buff);


}

void TestCBuffWrite()
{
	char str[] = "1234567890ABCDEFGHJIKLMNOPQRS";
	char dest[20];
	size_t i = 0;
	cbuff_t *my_buff = CBuffCreate(CAPACITEST);
	my_buff->w_ptr+=0;
	my_buff->r_ptr+=0;
	PRINT_TEST((CBuffWrite(my_buff, str, 20) == 10), "Write - return chars", 1);
	PRINT_TEST((CBuffRead(my_buff, dest, 20) == 10), "Read - return chars", 1);
	
	
	
}
