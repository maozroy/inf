/****************************************************************************
*										                                    *
*			                             	 		                        *
*		  	                OL79 - 31/10/19		                            *
*   					Auther: Erez Barr					                *	
*                  - Data Structures - Vector			                    * 
*                       Reviewer:    ?	                                    *
* 		                                                                    *
*****************************************************************************/
#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "vector.h" /*  */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RUN_TEST(TEST,NAME) \
	(TEST) ? \
	printf("Test"KBLU" %s"KGRN" PASS\n"KNRM, NAME) : \
	printf("Test"KBLU" %s"KRED" FAIL\n"KNRM, NAME);

#define CAP_SIZE 20
#define DATASIZE 4
#define MAX(x,y) ( (x) > (y) ? (x) : (y) )

int main()
{
	size_t test_size = MAX (CAP_SIZE, 1) ;
	size_t i = 0;
	void *i_pointer = &i;
	int check_counter = 0;
	

	d_vector_t *myvector = VectorCreate(CAP_SIZE,DATASIZE);

	printf("VectorCapacity : %ld\n",VectorCapacity(myvector));
	printf("test_size : %ld\n",test_size);

	RUN_TEST(test_size == VectorCapacity(myvector), "Vector Cap (min)")
	RUN_TEST(0 == VectorSize(myvector), "Vector Is Empty")

	for (i = 0; i < test_size; ++i)
	{
		VectorPushBack(myvector,i_pointer);	
	}

	for (i = 0; i < test_size; ++i)
	{
		check_counter = i - *(int *)(VectorGetItemAddress(myvector, i));
	}

	printf("VectorCapacity : %ld\n",VectorCapacity(myvector));
	printf("test_size : %ld\n",test_size);

	RUN_TEST(test_size == VectorCapacity(myvector), "Vector Cap (full)")
	RUN_TEST(test_size == VectorSize(myvector), "Vector Is full")
	RUN_TEST(0 == check_counter, "Entry Test 1")

	VectorPushBack(myvector,&test_size);

	RUN_TEST(test_size * 2 == VectorCapacity(myvector), "Vector Cap (full + 1)")
	RUN_TEST(test_size + 1 == VectorSize(myvector), "Vector Is full")		

	VectorPopBack(myvector);

	RUN_TEST(test_size * 2 == VectorCapacity(myvector), "Vector Cap (full + 1)")
	RUN_TEST(test_size == VectorSize(myvector), "Vector Is full")

	for (i = 0; i < test_size / 2; ++i)
	{
		VectorPopBack(myvector);	
	}

	for (i = 0; i < test_size / 2; ++i)
	{
		check_counter = i - *(int *)(VectorGetItemAddress(myvector, i));
	}
	RUN_TEST(0 == check_counter, "Entry Test 2")

	RUN_TEST(test_size * 2 == VectorCapacity(myvector), "Vector Cap (full + 1)")
	RUN_TEST(test_size / 2 == VectorSize(myvector), "Vector Is full")

		VectorPopBack(myvector);	

	RUN_TEST(test_size == VectorCapacity(myvector), "Vector Cap (full + 1)")
	RUN_TEST((test_size / 2) - 1 == VectorSize(myvector), "Vector Is full")

	VectorDestroy(myvector);

	return (0);
}

