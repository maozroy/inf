/****************************************************************************
*                                                                           *
*                               OL79 - 01/12/19                             *
*                               Auther - Erez                               *	
*                               Binary Sorting Tree 	                    *
*																			*
*****************************************************************************/


#include <stdlib.h> /* malloc */
#include <stdio.h>  /* printf */
#include <time.h>   /* clock  */
#include <string.h> /* memcmp */

#include "my_utils.h"
#include "avl.h"

#define ARR_SIZE (10)

int MyComparison(const void *x, const void *y);
int DoubleIntByTen(void *data, void *for_each_param);
/*************************************************/
int main ()
{
	void *param = NULL;
	avl_t *my_tree1 = AVLCreate(MyComparison);	
	avl_t *my_tree2 = AVLCreate(MyComparison);	
	int array[ARR_SIZE] = {6,3,9,1,0,2,7,4,5,8};
	int srray[ARR_SIZE] = {0,1,2,3,4,5,6,7,8,9};
	int srraycopy[ARR_SIZE] = {0,1,2,3,4,5,6,7,8,9};		
	int srray10[ARR_SIZE] = {0,10,20,30,40,50,60,70,80,90};
	int srrayuneven[ARR_SIZE/2] = {10,30,50,70,90};
	int srrayeven[ARR_SIZE/2]   = {0,20,40,60,80};		
	int i = 0;
	int test = 1;

	RUN_TEST(1 == AVLIsEmpty(my_tree1) && 1 == AVLIsEmpty(my_tree2) ,"New Tree Is Empty")
	RUN_TEST(0 == AVLSize(my_tree1) && 0 == AVLSize(my_tree2),"New Tree Is Size 0")
	RUN_TEST(0 == AVLGetHeight(my_tree1) && 0 == AVLGetHeight(my_tree2),"New Tree Is 0 Height")	
	
	for (i = 0; i < ARR_SIZE; ++i)
	{
		AVLInsert(my_tree1,&array[i]);
	}

	for (i = 0; i < ARR_SIZE; ++i)
	{
		AVLInsert(my_tree2,&srray[i]);
	}

	RUN_TEST(0 == AVLIsEmpty(my_tree1) && 0 == AVLIsEmpty(my_tree2),"New Tree Is Not Empty")
	RUN_TEST(ARR_SIZE == AVLSize(my_tree1) && ARR_SIZE == AVLSize(my_tree2),"New Tree Is Size ARR_ZISE")
	RUN_TEST(3 == AVLGetHeight(my_tree1) && 9 == AVLGetHeight(my_tree2),"New Tree Has Hight")
/*	
	printf("Height 1 = %d\n",AVLGetHeight(my_tree1));
	printf("Height 2 = %d\n",AVLGetHeight(my_tree2));	
*/

	for (i = 0; i < ARR_SIZE; ++i)
	{
		(i == *(int*)AVLFind(my_tree1, &srray[i]) ? (test = 1 * test) : (test = 0));
	/*	printf("Looked for %d found %d\n",i, *(int*)AVLFind(my_tree1, &srray[i])); */
	}

	for (i = 0; i < ARR_SIZE; ++i)
	{
		(i == *(int*)AVLFind(my_tree2, &srray[i]) ? (test = 1 * test) : (test = 0));
		
	/*	printf("Looked for %d found %d\n",i, *(int*)AVLFind(my_tree2, &srray[i]));	*/	
	}

	RUN_TEST(test,"Found All Data With Find")

	AVLForEach(my_tree1, DoubleIntByTen, param);
	AVLForEach(my_tree2, DoubleIntByTen, param);

	test = 1;
	for (i = 1; i < ARR_SIZE; ++i)
	{
		(NULL == AVLFind(my_tree1, &srraycopy[i]) ? (test = 1 * test) : (test = 0));
	/*	printf("Looked for (missing) %d found %p\n",i, AVLFind(my_tree1, &srraycopy[i])); */
	}

	for (i = 1; i < ARR_SIZE; ++i)
	{
		(NULL == AVLFind(my_tree2, &srraycopy[i]) ? (test = 1 * test) : (test = 0));
	/*	printf("Looked for (missing) %d found %p\n",i, AVLFind(my_tree1, &srraycopy[i])); */
	}

	for (i = 0; i < ARR_SIZE; ++i)
	{
		(i * 10 == *(int*)AVLFind(my_tree1, &srray10[i]) ? (test = 1 * test) : (test = 0));
	/*	printf("Looked for %d found %d\n",i* 10, *(int*)AVLFind(my_tree1, &srray10[i])); */
	}

	for (i = 0; i < ARR_SIZE; ++i)
	{
		(i * 10 == *(int*)AVLFind(my_tree2, &srray10[i]) ? (test = 1 * test) : (test = 0));
	/*	printf("Looked for %d found %d\n",i* 10, *(int*)AVLFind(my_tree2, &srray10[i]));	*/	
	}

	RUN_TEST(test,"Found All Data With Find After ForEach")
/*
disp *(int*)my_tree1->root->data
disp *(int*)my_tree1->root->children[0]->data
disp *(int*)my_tree1->root->children[1]->data
disp *(int*)my_tree1->root->children[0]->children[0]->data
disp *(int*)my_tree1->root->children[0]->children[1]->data
disp *(int*)my_tree1->root->children[1]->children[0]->data
disp *(int*)my_tree1->root->children[1]->children[1]->data
disp *(int*)my_tree1->root->children[0]->children[0]->children[0]->data
disp *(int*)my_tree1->root->children[0]->children[0]->children[1]->data
disp *(int*)my_tree1->root->children[0]->children[1]->children[0]->data
disp *(int*)my_tree1->root->children[0]->children[1]->children[1]->data
disp *(int*)my_tree1->root->children[1]->children[0]->children[0]->data
disp *(int*)my_tree1->root->children[1]->children[0]->children[1]->data
disp *(int*)my_tree1->root->children[1]->children[1]->children[0]->data
disp *(int*)my_tree1->root->children[1]->children[1]->children[1]->data

*/
		printf("height tree1:%ld, height tree2: %ld\n",
		AVLGetHeight(my_tree1), AVLGetHeight(my_tree2));
	test = 1;
	for (i = 0; i < ARR_SIZE/2; ++i)
	{
		AVLRemove(my_tree1, &srrayuneven[i]);
		AVLRemove(my_tree2, &srrayeven[i]);	
		printf("height tree1:%ld, height tree2: %ld\n",
		AVLGetHeight(my_tree1), AVLGetHeight(my_tree2));
		(ARR_SIZE - i - 1 == AVLSize(my_tree1)) ? (test = 1 * test) : (test = 0);		
		(ARR_SIZE - i - 1 == AVLSize(my_tree2)) ? (test = 1 * test) : (test = 0);				

	}

	RUN_TEST(test,"Check Size After Remove")

	test = 1;
	for (i = 0; i < ARR_SIZE/2; ++i)
	{
		(srrayeven[i] == *(int*)AVLFind(my_tree1, &srrayeven[i]) 
		? (test = 1 * test) : (test = 0));

	}
 
	for (i = 0; i < ARR_SIZE/2; ++i)
	{
		(srrayuneven[i] == *(int*)AVLFind(my_tree2, &srrayuneven[i]) 
		? (test = 1 * test) : (test = 0));
	}

	for (i = 0; i < ARR_SIZE/2; ++i)
	{
		(NULL == AVLFind(my_tree1, &srrayuneven[i]) ? (test = 1 * test) : (test = 0));
	}

	for (i = 0; i < ARR_SIZE/2; ++i)
	{
		(NULL == AVLFind(my_tree2, &srrayeven[i]) ? (test = 1 * test) : (test = 0));
	}
	
	RUN_TEST(test,"Found All Data With Find After Remove")

	AVLDestroy(my_tree1);
	AVLDestroy(my_tree2);
	
	return (0);
}
/*

disp *(int*)node->data
disp *(int*)data

*/
/*************/
int MyComparison(const void *x, const void *y)
{
	int answer = 0;

	if (*(int*)x > *(int*)y)
	{
		answer = 1;
	}

	if (*(int*)x < *(int*)y)
	{
		answer = -1;
	}

	return (answer);
}
/*************/
int DoubleIntByTen(void *data, void *for_each_param)
{
	UNUSED(for_each_param);
	*(int*)data *= 10;

	return (0);
}





















