
#include "avl.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

int my_cmp (const void *new_data, const void *src_data);
int action_func (void *data, void *for_each_param);

void TestCreateAndInsert(void);

int main()
{
	TestCreateAndInsert();

	return 0;
}


void TestCreateAndInsert(void)
{
	int arr[10] = {5, 10, 2, 8, 3, 7, 4, 6, 9, 20};
	int i = 0;
	int action_param = 20;
	
	avl_t *tree = AVLCreate(my_cmp);

	for (i = 0 ; i < 10 ; i++)
	{
		AVLInsert(tree, &arr[i]);
	}
	AVLForEach(tree, action_func, NULL);
	AVLRemove(tree, &arr[7]);
	PRINT_TEST(AVLFind(tree, &action_param) == &arr[9], "find", 0);
	printf("%ld\n",AVLSize(tree));
	printf("%ld\n",AVLGetHeight(tree));
	
	AVLForEach(tree, action_func, NULL);
	
	AVLDestroy(tree);


	
}

int action_func (void *data, void *for_each_param)
{
	printf("value is %d\n", *(int*)data);
	return 0;
}


int my_cmp (const void *new_data, const void *src_data)
{
	if (*(int *)new_data < *(int*)src_data)
	{
		return 1;
	}
	else if (*(int *)new_data > *(int*)src_data)
	{
		return -1;
	}
	return 0;

}
