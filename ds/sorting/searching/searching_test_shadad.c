#include <assert.h> /* assert */
#include <stddef.h>
#include <unistd.h>

#include "searching.h"

void BsearchTest();

int main()
{
    BsearchTest();

    return 0;
}

void BsearchTest()
{
    int arr_even[] = {1, 2, 3, 4, 5, 6};
    int arr_odd[] = {1, 2, 3, 4, 5};
    size_t arr_even_size = 6;
    size_t arr_odd_size = 5;
    size_t i = 0;
    int *ret_val = NULL;

    for (i = 0; i < arr_even_size; ++i)
    {
        ret_val = BSearch(arr_even, arr_even_size, arr_even[i]);
        assert(ret_val == &arr_even[i]);
    }

    ret_val = BSearch(arr_even, arr_even_size, 8);
    assert(NULL == ret_val);

    ret_val = BSearch(arr_even, arr_even_size, 0);
    assert(NULL == ret_val);

    ret_val = BSearch(arr_odd, arr_odd_size, 5);
        assert(ret_val == &arr_odd[4]);

    for (i = 0; i < arr_odd_size; ++i)
    {
        ret_val = BSearch(arr_odd, arr_odd_size, arr_odd[i]);
        assert(ret_val == &arr_odd[i]);
    }    
}
