 
#include "bst.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

int my_cmp (const void *new_data, 
							   const void *src_data, 
							   const void *comp_param);

void TestBSTCreate();

int main()
{
TestBSTCreate();
}

void TestBSTCreate()
{
	int x = 10;
	bst_t *my_tree = NULL;
	bst_iter_t *my_iter = NULL;

	my_tree = BSTCreate(my_cmp, NULL);
	my_iter = BSTInsert(my_tree, &x);
}

int my_cmp (const void *new_data, 
							   const void *src_data, 
							   const void *comp_param)
{
	return *(int *)new_data - *(int*)src_data;

}
