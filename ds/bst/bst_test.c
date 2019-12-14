 
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
	int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int i = 0;
	bst_t *my_tree = NULL;
	bst_iter_t my_iter[10] = {0};
	bst_iter_t new_iter = {0};

	my_tree = BSTCreate(my_cmp, NULL);
	printf("is it empty? %d\n",BSTIsEmpty(my_tree));
	for (i = 0 ; i < 10 ; i++)
	{
		my_iter[i] = BSTInsert(my_tree, &arr[i]);
	}
	printf("is it empty? %d\n",BSTIsEmpty(my_tree));
	
	new_iter = BSTEnd(my_tree);
	printf("%d",BSTIsSameIterator(new_iter, my_iter[9]));
	printf("is it the start? %d",BSTIsSameIterator(BSTBegin(my_tree), my_iter[0]));
}


int my_cmp (const void *new_data, 
							   const void *src_data, 
							   const void *comp_param)
{
	return *(int *)new_data - *(int*)src_data;

}
