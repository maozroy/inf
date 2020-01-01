
#include "heap.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

int my_comparison(const void *new_data, const void *src_data, void *compare_param);
int match(const void *new_data, const void *src_data);
void TestCreate();


int main()
{
	TestCreate();
	return 0;
}

void TestCreate()
{
	int arr[11] = {50,40,30,18,36,25,28,5,6,32,33};
	size_t i = 0;
	
	heap_t *heap = HeapCreate(my_comparison, NULL);
		printf("is it empty? is %d\n",HeapIsEmpty(heap));
	for (i = 0 ; i < 11 ; ++i)
	{
		HeapPush(heap, &arr[i]);
			PrintArrayIMP(heap);
			printf("END\n");
	}
				printf("Remove::\n");
	printf("%d\n",*(int*)HeapRemove(heap, match, &arr[5]));
	PrintArrayIMP(heap);
				printf("END\n");
	
	for (i = 0 ; i < 11 ; ++i)
	{
	HeapRemove(heap, match, &arr[i]);
			PrintArrayIMP(heap);
			printf("END\n");
	}

	/*printf("first peek is %d\n",*(int*)HeapPeek(heap));*/
	printf("size is %ld\n",HeapSize(heap));
	printf("is it empty? is %d\n",HeapIsEmpty(heap));
	HeapDestroy(heap);
	
	 

}
/*
disp **(int**)arr@arr_size


*/
int match(const void *new_data, const void *src_data)
{
	if (*(int*)new_data == *(int*)src_data)
	{
		return 1;
	}
	return 0;
}


int my_comparison(const void *new_data, const void *src_data, void *compare_param)
{
	(void)compare_param;
	if (*(int*)new_data >= *(int*)src_data)
	{
		return 1;
	}
	
	return 0;
}
