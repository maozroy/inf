#include <stdio.h> /* printf */

#include "avl.h"
#include "MyUtils.h" /* MAX2,MIN2 */

int MyCompareFuncIMP(const void *new_data, const void *src_data);
int MyForEachFunctionPrintData(void *data, void *for_each_param);

void TestCreateDestroy();
void TestInsert();
void TestFind();
void TestRemove();
void TestInsertRemove();

int main()
{
	TestCreateDestroy();
	TestInsert();
	TestFind();
	TestRemove();
	TestInsertRemove();	
	
	return 0;
}

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
	size_t height[10] = {0,1,1,2,2,2,2,3,3,3}; 
	size_t height1[10] = {0,1,1,2,2,2,3,3,3,3};
	size_t height2[10] = {0,1,1,2,2,2,3,3,3,3}; 
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
	printf("\n");
	
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
	printf("\n");
	
	AVLDestroy(new_avl);
	printf("\n\n");
	
	
	printf("!!! 	new avl 	!!!\n");
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < sizeof(arr)/sizeof(int); ++i)
	{
		printf("i = %u\n", i);
		PRINTTESTRESULTS("TestInsert_AVLIsEmpty",4*i + 1, (i == 0) == AVLIsEmpty(new_avl));
		PRINTTESTRESULTS("TestInsert_size",4*i + 2, i == AVLSize(new_avl));
		printf("insert %d\n", arr2[i]);
		PRINTTESTRESULTS("TestInsert_insert",4*i + 3, 0 == AVLInsert(new_avl, &arr2[i]));
		PRINTTESTRESULTS("TestInsert_AVLGetHeight",4*i + 4, height2[i] == AVLGetHeight(new_avl));
		printf("height = %ld, expected = %ld\n\n", AVLGetHeight(new_avl), height2[i]);
	}

	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	
	AVLDestroy(new_avl);
	printf("\n\n");
}

void TestRemove()
{
	int arr[] = {61,59,48,51,62,64,0,65,11,56,63};
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
	printf("\n");
	printf("remove right Leaf - number %d\n", arr[9]);
	AVLRemove(new_avl, &arr[9]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	printf("after remove of %d\n", arr[9]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	AVLDestroy(new_avl);
	
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	printf("remove left Leaf - number %d\n", arr[10]);
	AVLRemove(new_avl, &arr[10]);
	printf("after remove of %d\n", arr[10]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	AVLDestroy(new_avl);
	printf("\n\n");
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	printf("remove one with two chilren - number %d\n", arr[2]);
	AVLRemove(new_avl, &arr[2]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	printf("after remove of %d\n", arr[2]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	AVLDestroy(new_avl);
	printf("\n\n");
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	printf("remove one with two chilren - number %d\n", arr[5]);
	AVLRemove(new_avl, &arr[5]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	printf("after remove of %d\n", arr[5]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	AVLDestroy(new_avl);
	printf("\n\n");	
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	printf("remove root - number %d\n", arr[0]);
	AVLRemove(new_avl, &arr[0]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	printf("after remove of %d\n", arr[0]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	AVLDestroy(new_avl);
	printf("\n\n");	
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	printf("remove root - number %d\n", arr[8]);
	AVLRemove(new_avl, &arr[8]);
	printf("remove root - number %d\n", arr[9]);
	AVLRemove(new_avl, &arr[9]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 9 == AVLSize(new_avl));
	printf("after remove of %d and %d\n", arr[8], arr[9]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	AVLDestroy(new_avl);
	printf("\n\n");
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	for (i = 0; i < len; ++i)
	{
		AVLInsert(new_avl, &arr[i]);
	}
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	printf("remove root - number %d\n", arr[1]);
	AVLRemove(new_avl, &arr[1]);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	printf("after remove of %d\n", arr[1]);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
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

void TestInsertRemove()
{
	int a = 610;
	int b = 590;
	int c = 480;
	int d = 510;
	int e = 620;
	int f = 640;
	int g = 0;
	int h = 650;
	int k = 110;
	int l = 560;
	int m = 630;
	int n = 635;
	avl_t *new_avl = NULL;
	int param = 0;
	
	new_avl = AVLCreate(MyCompareFuncIMP);
	
	AVLInsert(new_avl, &a);
	AVLInsert(new_avl, &b);
	AVLInsert(new_avl, &c);
	AVLInsert(new_avl, &d);
	AVLInsert(new_avl, &e);
	AVLInsert(new_avl, &f);
	AVLInsert(new_avl, &g);
	AVLInsert(new_avl, &h);
	AVLInsert(new_avl, &k);
	AVLInsert(new_avl, &l);
	AVLInsert(new_avl, &m);
	
	printf("\nbefore remove\n");
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 11 == AVLSize(new_avl));
	printf("\n");
	
	printf("remove %d\n", a);
	AVLRemove(new_avl, &a);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	printf("after remove of %d\n", a);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	
	printf("remove %d\n", e);
	AVLRemove(new_avl, &e);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 9 == AVLSize(new_avl));
	printf("after remove of %d\n", e);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	
	printf("insert %d\n", n);
	AVLInsert(new_avl, &n);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 10 == AVLSize(new_avl));
	printf("after insert of %d\n", n);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	
	printf("remove %d\n", l);
	AVLRemove(new_avl, &l);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 9 == AVLSize(new_avl));
	printf("after remove of %d\n", l);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	
	printf("remove %d\n", d);
	AVLRemove(new_avl, &d);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 3 == AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 8 == AVLSize(new_avl));
	printf("after remove of %d\n", d);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	
	printf("remove %d\n", b);
	AVLRemove(new_avl, &b);
	PRINTTESTRESULTS("TestRemove_AVLGetHeight",1, 2 == AVLGetHeight(new_avl));
	printf("height = %ld, expected = 3\n" ,AVLGetHeight(new_avl));
	PRINTTESTRESULTS("TestRemove_size",1, 7 == AVLSize(new_avl));
	printf("after remove of %d\n", b);
	AVLForEach(new_avl, MyForEachFunctionPrintData, &param);
	printf("\n");
	
	AVLDestroy(new_avl);
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
	printf("%d, ", *(int *)data);
	
	return 0;
}

int MyForEachFunctionPrintHeight(void *data, void *for_each_param)
{
	(void)data;
	(void)for_each_param;
	printf("data = %d\n", *(int *)data);
	
	return 0;
}
