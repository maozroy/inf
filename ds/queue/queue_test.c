/****************************************************************************
*										                                    *
*			                             	 		                        *
*		  	                OL79 - 31/10/19		                            *
*   					Auther: Erez Barr					                *	
*                  - Data Structures - Queue			                    * 
* 		                                                                    *
*****************************************************************************/
#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "queue.h" /*  */


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

#define TESTSIZE 20
#define DATASIZE 8



int main()
{
	long int array1[TESTSIZE]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	long int array2[TESTSIZE]= {19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
	size_t i = 0;
	long int check_counter = 0;

	queue_t *myqueue = QCreate();
	queue_t *myqueuetwo = QCreate();


	RUN_TEST(1 == QIsEmpty(myqueue), "QueueIsEmpty")
	RUN_TEST(0 == QSize(myqueue), "TESTSIZE - empty")

	for (i = 0; i < TESTSIZE; ++i)
	{
		QEnqueue(myqueue,(long int*)&array1[i]);
		check_counter += (array1[0] - *(long int*)QPeek(myqueue));
	}

	RUN_TEST(0 == check_counter, "Push & Peek - fill the Queue")
	RUN_TEST(0 == QIsEmpty(myqueue), "QueueIsnotEmpty")
	RUN_TEST(TESTSIZE == QSize(myqueue), "TESTSIZE - Full")

	for (i = 0; i < TESTSIZE/2; ++i)
	{
		check_counter += (array1[i] - *(long int*)QPeek(myqueue));
		QDequeue(myqueue);
	}

	RUN_TEST(0 == check_counter, "Pop & Peek - empty half the Queue")
	RUN_TEST(0 == QIsEmpty(myqueue), "QueueIsnotEmpty")
	RUN_TEST(TESTSIZE/2 == QSize(myqueue), "TESTSIZE - Half")

	for (i = TESTSIZE/2; i < TESTSIZE; ++i)
	{
		check_counter += (array1[i] - *(long int*)QPeek(myqueue));
		QDequeue(myqueue);
	}

	RUN_TEST(0 == check_counter, "Pop & Peek - empty all the Queue")
	RUN_TEST(1 == QIsEmpty(myqueue), "QueueIsEmpty")
	RUN_TEST(0 == QSize(myqueue), "TESTSIZE - empty")

	for (i = 0; i < TESTSIZE; ++i)
	{
		QEnqueue(myqueue,(long int*)&array1[i]);
		check_counter += (array1[0] - *(long int*)QPeek(myqueue));
	}

	RUN_TEST(0 == check_counter, "Push & Peek - fill the Queue again")
	RUN_TEST(0 == QIsEmpty(myqueue), "QueueIsnotEmpty again")
	RUN_TEST(TESTSIZE == QSize(myqueue), "TESTSIZE - Full again")

	for (i = 0; i < TESTSIZE; ++i)
	{
		QEnqueue(myqueuetwo,(long int*)&array2[i]);
		check_counter += (array2[0] - *(long int*)QPeek(myqueuetwo));
	}

	RUN_TEST(0 == check_counter, "Push & Peek - fill the 2nd Queue")
	RUN_TEST(0 == QIsEmpty(myqueuetwo), "2nd QueueIsnotEmpty")
	RUN_TEST(TESTSIZE == QSize(myqueuetwo), "2nd TESTSIZE - Full")

	QAppend(myqueue,myqueuetwo);

	RUN_TEST(TESTSIZE*2 == QSize(myqueue), "Both TESTSIZE - Full") /*****/
	RUN_TEST(0 == QIsEmpty(myqueue), "Both QueueIsnotEmpty")

	for (i = 0; i < TESTSIZE; ++i)
	{
		check_counter += (array1[i] - *(long int*)QPeek(myqueue));
		QDequeue(myqueue);
	}

	for (i = 0; i < TESTSIZE/2; ++i)
	{
		check_counter += (array2[i] - *(long int*)QPeek(myqueue));
		QDequeue(myqueue);
	}

	RUN_TEST(0 == check_counter, "Pop & Peek - empty 3/4 of both Queue")
	RUN_TEST(0 == QIsEmpty(myqueue), "QueueIsEmpty")
	RUN_TEST(TESTSIZE/2 == QSize(myqueue), "TESTSIZE - empty")

	QDestroy(myqueue); 
	return (0);
}












