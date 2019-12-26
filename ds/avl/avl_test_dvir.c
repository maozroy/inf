/********************************************
 *              OL-79 - ds                  *
 *       	  AVL Tree Tests  				*
 *                                          *
 *    										*
 *               24/12/19                   *
 ********************************************/

#include <stdio.h>      /* For printf	*/ 
#include <stdlib.h>     /* for malloc	*/

#include "avl.h"
#include "myutils.h"

#define ARRAY_SIZE 20
#define RAND_RANGE 100
#define TEST_NUM 50


void AVLCreateTest();
void AVLInsertTest();
void AVLRemoveTest();
void AVLFindTest();
void AVLForEachTest();
void AVLSizeIsEmptyTest();
void AVLInsBalanceTest();
void AVLRmvBalanceTest();

static int AddToDataIMP(void *data, void *param);

int main ()
{	
	VLGCHK;
	AVLCreateTest();	
	AVLInsertTest();
	AVLRemoveTest();
	AVLFindTest();
	AVLForEachTest();
	AVLSizeIsEmptyTest();
	AVLInsBalanceTest();
	AVLRmvBalanceTest();
	
	return 0;
} 

static int AscendingCmpIMP(const void *new_data,
						   const void *src_data)
{
	int new_num = *(int *)new_data;
	int src_num = *(int *)src_data;
	int diff = new_num - src_num;
	
	if ( diff > 0 )
	{	
		return 1;
	}
	else if ( diff < 0 )
	{
		return -1;
	}
	else
	{	
		return 0;	
	}	
}

void AVLCreateTest()	
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	
	int test1 = ( NULL != test_avl );
	int test2 = ( AVLIsEmpty(test_avl) );
	
	PRINT_TEST(test1, "AVLCreate", 1);
	PRINT_TEST(test2, "AVLCreate", 2);
	printf("\n");	
	
	AVLDestroy(test_avl);
}
	
void AVLInsertTest()
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	int array[16] = {15, 7, 20, 30, 17, 3, 10, -7, 12, 9, 18, 16, 13, -4, -15, 19};
	size_t i = 0;
	int test1 = 0;
	int test2 = 0;
	int result = FAILURE;
	
	for (; i < 16; ++i)
	{
		result = AVLInsert(test_avl, &array[i]);
		if ( FAILURE == result )
			break;
	}

	test1 = ( (SUCCESS == result) && (!AVLIsEmpty(test_avl)) );
	test2 = ( (4 == AVLGetHeight(test_avl)) && (16 == AVLSize(test_avl)) );
	
	PRINT_TEST(test1, "AVLInsert", 1);
	PRINT_TEST(test2, "AVLInsert", 2);
	printf("\n");	
	
	AVLDestroy(test_avl);
}

void AVLRemoveTest()
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	int array[16] = {15, 7, 20, 30, 17, 3, 10, -7, 12, 9, 18, 16, 13, -4, -15, 19};
	size_t i = 0;
	int test1 = 0;
	int test2 = 0;
	
	AVLInsert(test_avl, &array[0]);
	AVLInsert(test_avl, &array[1]);
	AVLRemove(test_avl, &array[1]);
	
	test1 = ( (1 == AVLSize(test_avl)) && (0 == AVLGetHeight(test_avl)) );
	
	for (i = 1; i < 16; ++i)
	{
		AVLInsert(test_avl, &array[i]);
	}
	
	AVLRemove(test_avl, &array[12]);
	AVLRemove(test_avl, &array[15]);
	
	test2 = ( 14 == AVLSize(test_avl) && (4 == AVLGetHeight(test_avl)) );
	
	PRINT_TEST(test1, "AVLRemove", 1);
	PRINT_TEST(test2, "AVLRemove", 2);
	printf("\n");	
	
	AVLDestroy(test_avl);
}

void AVLFindTest()
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	int array[16] = {15, 7, 20, 30, 17, 3, 10, -7, 12, 9, 18, 16, 13, -4, -15, 19};
	size_t i = 0;
	int test1 = 0;
	int test2 = 0;
	
	for (; i < 16; ++i)
	{
		AVLInsert(test_avl, &array[i]);
	}
	
	test1 = ( 9 == *(int *)AVLFind(test_avl, &array[9]) );
	test2 = ( NULL == AVLFind(test_avl, &test2) );
	
	PRINT_TEST(test1, "AVLFind", 1);
	PRINT_TEST(test2, "AVLFind", 2);
	printf("\n");	
	
	AVLDestroy(test_avl);	
}

void AVLForEachTest()
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	int array[16] = {15, 7, 20, 30, 17, 3, 10, -7, 12, 9, 18, 16, 13, -4, -15, 19};
	size_t i = 0;
	int test1 = 2;
	int test2 = 7;
	
	
	for (; i < 16; ++i)
	{
		AVLInsert(test_avl, &array[i]);	
	}
	
	test1 = ( SUCCESS == AVLForEach(test_avl, AddToDataIMP, &test1) );
	test2 = ( (18 == *(int *)AVLFind(test_avl, &array[11])) && 
			  (NULL == AVLFind(test_avl, &test2)) );
	
	PRINT_TEST(test1, "AVLForEach", 1);
	PRINT_TEST(test2, "AVLForEach", 2);
	printf("\n");	
	
	AVLDestroy(test_avl);
}
	
static int AddToDataIMP(void *data, void *param)
{
	*(int *)data += *(int *)param;
	
	return SUCCESS;
}
	
void AVLSizeIsEmptyTest()
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	int array[16] = {15, 7, 20, 30, 17, 3, 10, -7, 12, 9, 18, 16, 13, -4, -15, 19};
	size_t i = 0;
	int test1 = 0;
	int test2 = 0;
	
	test1 = ( AVLIsEmpty(test_avl) && (0 == AVLSize(test_avl)) );
	
	for (; i < 16; ++i)
	{
		AVLInsert(test_avl, &array[i]);	
	}
	
	test2 = ( 16 == AVLSize(test_avl) );

	PRINT_TEST(test1, "AVLIsEmpty", 1);
	PRINT_TEST(test2, "AVLSize", 1);
	printf("\n");	
	
	AVLDestroy(test_avl);
}
	
void AVLInsBalanceTest()
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	int array[16] = {15,10,5,3,1,18,21,24,9,11,12,13,6,7,8,14};
	size_t i = 0;
	int test1 = 0;
	int test2 = 0;
	int result = FAILURE;
	
	for (; i < 16; ++i)
	{
		result = AVLInsert(test_avl, &array[i]);
		if ( FAILURE == result )
			break;
	}

	test1 = ( (SUCCESS == result) && (9 == *(int *)AVLFind(test_avl, &array[8])) );
	test2 = ( (4 == AVLGetHeight(test_avl)) && (16 == AVLSize(test_avl)) );
	
	PRINT_TEST(test1, "AVLInsBalance", 1);
	PRINT_TEST(test2, "AVLInsBalance", 2);
	printf("\n");	
	
	AVLDestroy(test_avl);	
}


void AVLRmvBalanceTest()
{
	avl_t *test_avl = AVLCreate(AscendingCmpIMP);
	int array[16] = {15,10,5,3,1,18,21,24,9,11,12,13,6,7,8,14};
	size_t i = 0;
	int test1 = 0;
	int test2 = 0;
	int result = FAILURE;
	
	for (; i < 16; ++i)
	{
		result = AVLInsert(test_avl, &array[i]);
		if ( FAILURE == result )
			break;
	}
	
	AVLRemove(test_avl, &array[9]);
	AVLRemove(test_avl, &array[10]);
	
	printf("%ld\n", AVLSize(test_avl));
	test1 = ( (14 == AVLSize(test_avl)) && (3 == AVLGetHeight(test_avl)) && 
			  (NULL == AVLFind(test_avl, &array[9])) );
	
	PRINT_TEST(test1, "AVLRmvBalance", 1);
	PRINT_TEST(test2, "AVLRmvBalance", 2);
	printf("\n");	
	
	AVLDestroy(test_avl);	



}
	
