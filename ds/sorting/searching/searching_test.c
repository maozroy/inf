#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

#include "searching.h"
#define NUM_OF_ELEMENTS 25


int main ()
{
	int arr1[NUM_OF_ELEMENTS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
					11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
	size_t i = 0;
	for (i = 0; i < NUM_OF_ELEMENTS ; i++)
	{
		PRINT_TEST(*(int*)JSearch(arr1, NUM_OF_ELEMENTS, i) == i, "search", i);
	}	

return 0;
}
