#include <stdio.h> /* printf */

#include "avl.h"
#include "MyUtils.h" /* MAX2,MIN2 */

int MyCompareFuncIMP(const void *new_data, const void *src_data);
int MyForEachFunctionPrintData(void *data, void *for_each_param);

void TestCreateDestroy();
void TestInsert();
void TestFind();
void TestRemove();

int main()
{
/*	TestCreateDestroy();
*/	TestInsert();
/*	TestFind();*/
	TestRemove();
	
	return 0;
}
/*
display *(int *)node->data
display *(int *)node->children[0]->data
display *(int *)node->children[1]->data
display *(int *)node->children[0]->children[0]->data
display *(int *)node->children[0]->children[1]->data



*/
void TestCreateDestroy()
{
	avl_t *new_avl = AVLCreate(MyCompareFuncIMP);
	AVLDestroy(new_avl);
	printf("Create AVL\n");
	printf("Destroy AVL\n");
	
	printf("\n\n");
}

void TestInsert()
{
	int arr[10] = {5,3,-8,-5,6,8,-56,9,-45,0};
	int arr1[10] = {20,10,40,60,30,25,27,9,29,23};
	int arr2[10] = {10,20,15,60,30,25,27,9,29,23};
	int arr3[10] = {0};
	size_t height[10] = {0,1,1,2,2,2,2,3,3,3}; 
	size_t height1[10] = {0,1,1,2,2,2,3,3,3,3}; 
	size_t height2[10] = {0,1,1,2,2,2,2,3,3,3}; 
	size_t height3[10] = {0,1,1,2,2,2,2,3,3,3}; 
	avl_t *new_avl = NULL;
	unsigned int i = 0;
	int param = 0;
	
	printf("!!! 	new avl 	!!!\n");
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		PRINTTESTRESULTS("TestInsert_AVLIsEmpty",4*i + 1, (i == 0) == AVLIsEmpty(new_avl));
		PRINTTESTRESULTS("TestInsert_size",4*i + 2, i == AVLSize(new_avl));
		printf("insert %d\n", arr[i]);
		PRINTTESTRESULTS("TestInsert_insert",4*i + 3, 0 == AVLInsert(new_avl, &arr[i]));
		PRINTTESTRESULTS("TestInsert_AVLGetHeight",4*i + 4, height[i] == AVLGetHeight(new_avl));
		printf("height = %ld, expected = %ld\n\n", AVLGetHeight(new_avl), height[i]);
	}

	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	
	AVLDestroy(new_avl);
	printf("\n\n");
	
	printf("!!! 	new avl 	!!!\n");
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		PRINTTESTRESULTS("TestInsert_AVLIsEmpty",4*i + 1, (i == 0) == AVLIsEmpty(new_avl));
		PRINTTESTRESULTS("TestInsert_size",4*i + 2, i == AVLSize(new_avl));
		printf("insert %d\n", arr1[i]);
		PRINTTESTRESULTS("TestInsert_insert",4*i + 3, 0 == AVLInsert(new_avl, &arr1[i]));
		PRINTTESTRESULTS("TestInsert_AVLGetHeight",4*i + 4, height1[i] == AVLGetHeight(new_avl));
		printf("height = %ld, expected = %ld\n\n", AVLGetHeight(new_avl), height1[i]);
	}

	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	
	AVLDestroy(new_avl);
	printf("\n\n");
	
	
	printf("!!! 	new avl 	!!!\n");
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < /*sizeof(arr)/sizeof(int)*/3; ++i)
	{
		PRINTTESTRESULTS("TestInsert_AVLIsEmpty",4*i + 1, (i == 0) == AVLIsEmpty(new_avl));
		PRINTTESTRESULTS("TestInsert_size",4*i + 2, i == AVLSize(new_avl));
		printf("insert %d\n", arr2[i]);
		PRINTTESTRESULTS("TestInsert_insert",4*i + 3, 0 == AVLInsert(new_avl, &arr2[i]));
		PRINTTESTRESULTS("TestInsert_AVLGetHeight",4*i + 4, height2[i] == AVLGetHeight(new_avl));
		printf("height = %ld, expected = %ld\n\n", AVLGetHeight(new_avl), height2[i]);
	}

	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	
	AVLDestroy(new_avl);
	printf("\n\n");
}

void TestRemove()
{
	int arr[] = {61,59,48,51,62,64,0,67,21,56,63};
	avl_t *new_avl = NULL;
	unsigned int i = 0;
	int param = 0;
	size_t len = sizeof(arr)/sizeof(int);
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("remove right Leaf - number %d\n", arr[9]);
	AVLRemove(new_avl, &arr[9]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	printf("after remove of %d\n", arr[9]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	AVLDestroy(new_avl);
	
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("remove left Leaf - number %d\n", arr[10]);
	AVLRemove(new_avl, &arr[10]);
	printf("after remove of %d\n", arr[10]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	AVLDestroy(new_avl);
	printf("\n\n");
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("remove one with two chilren - number %d\n", arr[2]);
	AVLRemove(new_avl, &arr[2]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	printf("after remove of %d\n", arr[2]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	AVLDestroy(new_avl);
	printf("\n\n");
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("remove one with two chilren - number %d\n", arr[5]);
	AVLRemove(new_avl, &arr[5]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	printf("after remove of %d\n", arr[5]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	AVLDestroy(new_avl);
	printf("\n\n");	
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("remove root - number %d\n", arr[0]);
	AVLRemove(new_avl, &arr[0]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	printf("after remove of %d\n", arr[0]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	AVLDestroy(new_avl);
	printf("\n\n");	
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("remove root - number %d\n", arr[8]);
	AVLRemove(new_avl, &arr[8]);
	printf("remove root - number %d\n", arr[9]);
	AVLRemove(new_avl, &arr[9]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	printf("after remove of %d\n", arr[0]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	AVLDestroy(new_avl);
	printf("\n\n");
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("remove root - number %d\n", arr[1]);
	AVLRemove(new_avl, &arr[1]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	printf("after remove of %d\n", arr[0]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	AVLDestroy(new_avl);
	printf("\n\n");	
}

void TestFind()
{
	int arr[] = {5,3,-8,-5,6,8,-56,9,-45,0};
	int missing_value1 = 201;
	int missing_value2 = -201;
	int missing_value3 = 7;
	avl_t *new_avl = AVLCreate(MyCompareFuncIMP);
	unsigned int i = 0;
	
	for (i = 0; i < 10; ++i)
	{
		PRINTTESTRESULTS("TestFind_insert", i + 1, 0 == AVLInsert(new_avl, &arr[i]));
	}
	
	for (i = 0; i < 10; ++i)
	{
		PRINTTESTRESULTS("TestFind_find",i + 11, 
						  arr[i] == *(int *)AVLFind(new_avl, &arr[i]));
	}
	
	PRINTTESTRESULTS("TestFind_find",i + 21, 
						  NULL == AVLFind(new_avl, &missing_value1));
	PRINTTESTRESULTS("TestFind_find",i + 22, 
						  NULL == AVLFind(new_avl, &missing_value2));
	PRINTTESTRESULTS("TestFind_find",i + 23, 
						  NULL == AVLFind(new_avl, &missing_value3));				  					  
	
	AVLDestroy(new_avl);
	printf("\n\n");
}

int MyCompareFuncIMP(const void *new_data, const void *src_data)
{
	if (*(int *)new_data > *(int *)src_data)
	{
		return 1;
	}
	else if (*(int *)new_data < *(int *)src_data)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int MyForEachFunctionPrintData(void *data, void *for_each_param)
{
	(void)data;
	(void)for_each_param;
	printf("data = %d\n", *(int *)data);
	
	return 0;
}

int MyForEachFunctionPrintHeight(void *data, void *for_each_param)
{
	(void)data;
	(void)for_each_param;
	printf("data = %d\n", *(int *)data);
	
	return 0;
}
