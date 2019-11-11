
#include "dllist.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) \

void TestDLListCreate(void);
void TestDLListPushBack(void);


int main()
{

	TestDLListCreate();
	TestDLListPushBack();
	
	return 0;
}

void TestDLListCreate(void)
{
	dl_list_t *my_list = DLListCreate();
	DLListDestroy(my_list);
}

void TestDLListPushBack(void)
{
	dll_iter_t first_node= NULL;
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;

	dl_list_t *my_list = DLListCreate();
	first_node = DLListPushBack(my_list, &num1);
	first_node = DLListPushBack(my_list, &num2);
	first_node = DLListPushBack(my_list, &num3);
		first_node = DLListPushBack(my_list, &num4);
	DLListInsert(&num5, first_node, my_list);
	PrintIntList(my_list);
	DLListSplice(DLListBegin(my_list), DLListNext(DLListBegin(my_list)), DLListEnd(my_list));


	PrintIntList(my_list);

	DLListDestroy(my_list);
	

}


