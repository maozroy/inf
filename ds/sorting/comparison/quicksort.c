#include <stddef.h> /*size_t*/
#include <string.h> /* memcpy */
#include <alloca.h> /* alloca */

#include "../sorting.h"

#define PIVOT 0
#define JUMP(i) (((char *)base) + ((i) * (elem_size)))

static void SwapIMP(void *val1, void *val2, size_t element_size);

void QuickSort(void *base, size_t nmemb, size_t elem_size, is_before_t func)
{
	size_t pivot_i = 0;
	size_t right_i = 0;
	size_t left_i = 0;
	
	if (nmemb <= 1)
	{
		return;
	}
	
	right_i = nmemb - 1;
	left_i = 1;

	while (left_i <= right_i)
	{
		while ((func(JUMP(right_i), JUMP(PIVOT), NULL) <= 0)
			    && (right_i > 0))
		{
			--right_i;
		}
		
		while ((func(JUMP(left_i), JUMP(PIVOT), NULL) > 0)
				&& (left_i < nmemb))
		{
			++left_i;
		}
		
		if (right_i > left_i)
		{
			SwapIMP(JUMP(right_i), JUMP(left_i), elem_size);
			--right_i;
			++left_i;			
		}
	}
	SwapIMP(JUMP(pivot_i), JUMP(right_i), elem_size);
	
	QuickSort(base, right_i, elem_size, func);
	QuickSort(JUMP(left_i), nmemb - left_i, elem_size, func);
}

static void SwapIMP(void *val1, void *val2, size_t element_size)
{
    char *temp = (char *)alloca(element_size);

    memcpy(temp, val1, element_size);
    memcpy(val1, val2, element_size);
    memcpy(val2, temp, element_size);
}
