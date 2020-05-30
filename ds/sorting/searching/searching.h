/****************************************************************************
*                                                                           *
*                               OL79 - 06/01/20                             *
*                                                                           *	
*                                    Searching                              *
*                                                                           *
*****************************************************************************/

/* The function searches a given element in an array using binary search */
/* The function returns a pointer to the matched element.
    If not found returns NULL */
int *BSearch(const int *arr, size_t size, int val);

/* The function searches a given element in an array using jump search */
/* The function returns a pointer to the matched element.
    If not found returns NULL */
int *JSearch(const int *arr, size_t size, int val);
