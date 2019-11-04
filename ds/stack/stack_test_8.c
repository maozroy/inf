/****************************************************************************
*										                                    *
*			                             	 		                        *
*		  	                OL79 - 31/10/19		                            *
*   					Auther: Erez Barr					                *	
*                  - Data Structures - Stack			                    * 
*                       Reviewer:    ?	                                    *
* 		                                                                    *
*****************************************************************************/
#include <stdio.h> /* printf */
#include <string.h> /* memset */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "stack.h" /*  */

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
	printf("Test"KBLU"%s"KRED" FAIL\n"KNRM, NAME);

#define STACKSIZE 20
#define DATASIZE 5


int main()
{
	int array1[STACKSIZE]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	int array2[STACKSIZE]= {19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
	int i = 0;
	int check_counter = 0;

	stack_t *mystack = (stack_t*)StackCreate(STACKSIZE,DATASIZE);


	RUN_TEST(1 == StackIsEmpty(mystack), "StackIsEmpty")
	RUN_TEST(0 == StackSize(mystack), "StackSize - 0")

	for (i = 0; i < STACKSIZE; ++i)
	{
		StackPush(mystack,(int*)&array1[i]);
		check_counter += (array1[i] - *(unsigned int*)StackPeek(mystack));
	}
	RUN_TEST(0 == check_counter, " Push & Peek - fill the stack")
	RUN_TEST(0 == StackIsEmpty(mystack), "StackIsnotEmpty")
	RUN_TEST(STACKSIZE == StackSize(mystack), "StackSize - Full")

	for (i = 0; i < STACKSIZE/2; ++i)
	{
		check_counter += (array2[i] - *(int*)StackPeek(mystack));
		StackPop(mystack);
	}

	RUN_TEST(0 == check_counter, " Push & Peek - empty half the stack")
	RUN_TEST(0 == StackIsEmpty(mystack), "StackIsnotEmpty")
	RUN_TEST(STACKSIZE/2 == StackSize(mystack), "StackSize - Half")

	for (i = STACKSIZE/2; i < STACKSIZE; ++i)
	{
		check_counter += (array2[i] - *(int*)StackPeek(mystack));
		StackPop(mystack);
	}

	RUN_TEST(0 == check_counter, " Push & Peek - empty all the stack")
	RUN_TEST(1 == StackIsEmpty(mystack), "StackIsEmpty")
	RUN_TEST(0 == StackSize(mystack), "StackSize - empty")

	StackDestroy(mystack);

	return (0);
}











