#include <stddef.h> /*size_t*/
#include <string.h> /* memcpy */
#include <alloca.h> /* alloca */

#include "../sorting.h"

#define PIVOT 0
#define LAST_ELEMENT ((nmemb) - (1))
#define JUMP(i) (((char *)base) + ((i) * (elem_size)))

static void SwapIMP(void *val1, void *val2, size_t element_size);

void QuickSort(void *base, size_t nmemb, size_t elem_size, is_before_t compare)
{
	size_t pivot_i = 0;
	size_t right_i = LAST_ELEMENT;
	size_t left_i = 1;
	
	if (nmemb <= 1)
	{
		return;
	}
	
	while (left_i <= right_i)
	{
		while ((compare(JUMP(right_i), JUMP(PIVOT)) <= 0)
			    && (right_i > 0))
		{
			--right_i;
		}
		
		while ((compare(JUMP(left_i), JUMP(PIVOT)) > 0)
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
	
	QuickSort(base, right_i, elem_size, compare);
	QuickSort(JUMP(left_i), (nmemb - left_i), elem_size, compare);
}

static void SwapIMP(void *val1, void *val2, size_t element_size)
{
    char *temp = (char *)alloca(element_size);

    memcpy(temp, val1, element_size);
    memcpy(val1, val2, element_size);
    memcpy(val2, temp, element_size);
}
