#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#include "sorting.h"

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 
int comparefunc(const void *x, const void *y);


int main()
{
	int hundred_arr[100]      = {0};
	int hundred_arr_qs[100]   = {0};
	int hundrer_arr_res[5]  = {0};
	int one_arr[1]           = {0};
	int one_arr_res[1]           = {0};
	int one_arr_qs[1]        = {0};
	int five_arr[5]          = {0};
	int five_arr_res[5]          = {0};
	int five_arr_qs[5]       = {0};
	int thousand_arr[1000]    = {0};
	int thousand_arr_res[1000]    = {0};
	int thousand_arr_qs[1000] = {0};
	int i = 0;
    srand(time(NULL));
    
    for (i = 0 ; i < 100 ; i++)
    {
    	hundred_arr[i] = (rand()%100);
    	hundred_arr_qs[i] = hundred_arr[i];
    }
    
    hundred_arr[2] = 101;
    hundred_arr_qs[2] = 101;
    
    hundred_arr[1] = 0;
    hundred_arr_qs[1] = 0;
    
    for (i = 0 ; i < 1 ; i++)
    {
    	one_arr[i] = (rand()%1);
    	one_arr_qs[i] = one_arr[i];
    }
    
     for (i = 0 ; i < 5 ; i++)
    {
    	five_arr[i] = (rand()%100);
    	five_arr_qs[i] = five_arr[i];
    } 
    
    five_arr[2] = 101;
    five_arr_qs[2] = 101;
    
    five_arr[1] = 0;
    five_arr_qs[1] = 0;
 

    for (i = 0 ; i < 1000 ; i++)
    {
    	thousand_arr[i] = (rand()%1000);
    	thousand_arr_qs[i] = thousand_arr[i];
    } 
    
    thousand_arr[2] = 1000;
    thousand_arr_qs[2] = 1000;
    
    thousand_arr[1] = 0;
    thousand_arr_qs[1] = 0;
    

  /*
  	CountingSort(hundred_arr, 100, 0, 5, hundrer_arr_res);
  	CountingSort(thousand_arr, 1000, 0, 1000, thousand_arr_res);
  	CountingSort(five_arr, 5, 0, 101, five_arr_res);
  	CountingSort(one_arr, 1, 1, 1, one_arr);
  */	

    MergeSort(hundred_arr, 100);
  	MergeSort(thousand_arr, 1000);
  	MergeSort(five_arr, 5);
  	MergeSort(one_arr, 1);

    
    
    qsort((void *)hundred_arr_qs, 100, 4, comparefunc);
    qsort((void *)one_arr_qs, 1, 4, comparefunc);
    qsort((void *)five_arr_qs, 5, 4, comparefunc);
    qsort((void *)thousand_arr_qs, 1000, 4, comparefunc);
    
    PRINT_TEST(memcmp(hundred_arr, hundred_arr_qs, sizeof(hundred_arr)) == 0,"100 test", 1);
	PRINT_TEST(memcmp(one_arr, one_arr_qs, sizeof(one_arr)) == 0,"1 test", 1);
	PRINT_TEST(memcmp(five_arr, five_arr_qs, sizeof(five_arr)) == 0,"5 test", 1);
	PRINT_TEST(memcmp(thousand_arr, thousand_arr_qs, sizeof(thousand_arr)) == 0,"1000 test", 1);
    
	/*
	PRINT_TEST(memcmp(hundrer_arr_res, hundred_arr_qs, sizeof(hundred_arr)) == 0,"100 test", 1);
	PRINT_TEST(memcmp(one_arr_res, one_arr_qs, sizeof(one_arr)) == 0,"1 test", 1);
	PRINT_TEST(memcmp(five_arr_res, five_arr_qs, sizeof(five_arr)) == 0,"5 test", 1);
	PRINT_TEST(memcmp(thousand_arr_res, thousand_arr_qs, sizeof(thousand_arr)) == 0,"1000 test", 1);
	*/
	return 0;	
}




int comparefunc(const void *x, const void *y)
{
	if(*(int *)x > *(int *)y)
	{
		return 1;
	}
	return 0;
}
