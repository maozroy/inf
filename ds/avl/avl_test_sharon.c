#include <stdio.h> /* printf */
#include <string.h>

#include "avl.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

void TestAVLCreate();
void TestAVLInsert();
void TestAVLFind();
void TestAVLForEach();
void TestAVLIsEmpty();
void TestAVLSize();
void TestAVLGetHeight();
void TestAVLRemove();
int ComparFunc(const void *new_data, const void *src_data);
int PrintData(void *data, void *for_each_param);

int main()
{   
    TestAVLCreate();
    TestAVLInsert();
    TestAVLFind();
    TestAVLForEach();
    TestAVLIsEmpty();
    TestAVLSize();
    TestAVLGetHeight();
    TestAVLRemove();
    
	return 0;
}

void TestAVLCreate()
{
	avl_t *new_avl = AVLCreate(ComparFunc);
	printf("TestAVLCreate\n");
	TEST2(new_avl);
	AVLDestroy(new_avl);  
}

void TestAVLInsert()
{
    avl_t *new_avl = AVLCreate(ComparFunc);
	int a = 10;
	int b = 7;
	int c = 13;
	int d = 12;
	int e = 9;
	int f = 20;
	
	printf("TestAVLInsert\n");
	AVLInsert(new_avl, &a);
	TEST1(AVLFind(new_avl, &a), &a);
	TEST1(AVLFind(new_avl, &e), NULL);
	AVLInsert(new_avl, &b);
	AVLInsert(new_avl, &c);
	AVLInsert(new_avl, &d);
	AVLInsert(new_avl, &e);
	AVLInsert(new_avl, &f);
	
	AVLForEach(new_avl, PrintData, NULL);
		
	AVLDestroy(new_avl);
}

void TestAVLFind()
{
    avl_t *new_avl = AVLCreate(ComparFunc);
	int a = 10;
	int b = 7;
	int c = 13;
	int d = 12;
	int e = 9;
	int data_to_find = 12;
	printf("TestAVLFind\n");
	AVLInsert(new_avl, &a);
	AVLInsert(new_avl, &b);
	AVLInsert(new_avl, &c);
	AVLInsert(new_avl, &d);
	TEST1(AVLFind(new_avl, &data_to_find), &d);
	TEST1(AVLFind(new_avl, &e), NULL);
	AVLDestroy(new_avl);
}

void TestAVLForEach()
{
    avl_t *new_avl = AVLCreate(ComparFunc);
	int a = 10;
	int b = 7;
	int c = 13;
	int d = 12;
	int e = 9;
	int f = 20;
	int return_val = 0;
    printf("TestAVLForEach\n");
	AVLInsert(new_avl, &a);
	AVLInsert(new_avl, &b);
	AVLInsert(new_avl, &c);
	AVLInsert(new_avl, &d);
	AVLInsert(new_avl, &e);
	AVLInsert(new_avl, &f);
	return_val = AVLForEach(new_avl, PrintData, NULL);
	TEST1(0, return_val);
	AVLDestroy(new_avl);
}

void TestAVLIsEmpty()
{
	avl_t *new_avl = AVLCreate(ComparFunc);
	int a = 10;
	printf("TestAVLIsEmpty\n");
	TEST1(AVLIsEmpty(new_avl), 1);
	AVLInsert(new_avl, &a);
	TEST1(AVLIsEmpty(new_avl), 0);
	AVLDestroy(new_avl);
}

void TestAVLSize()
{
    avl_t *new_avl = AVLCreate(ComparFunc);
	int a = 10;
	int b = 7;
	int c = 13;
    printf("TestAVLSize\n");
    TEST1(0, AVLSize(new_avl));
	AVLInsert(new_avl, &a);
	TEST1(1, AVLSize(new_avl));
	AVLInsert(new_avl, &b);
	TEST1(2, AVLSize(new_avl));
	AVLInsert(new_avl, &c);
	TEST1(3, AVLSize(new_avl));

	AVLDestroy(new_avl);
}

void TestAVLGetHeight()
{
    avl_t *new_avl = AVLCreate(ComparFunc);
	int a = 10;
	int b = 7;
	int c = 13;
	int d = 12;
	int e = 9;
	int f = 20;
	
	printf("TestAVLGetHeight\n");
	AVLInsert(new_avl, &a);
	TEST1(0, AVLGetHeight(new_avl));
	AVLInsert(new_avl, &b);
	TEST1(1, AVLGetHeight(new_avl));
	AVLInsert(new_avl, &c);
	TEST1(1, AVLGetHeight(new_avl));
	AVLInsert(new_avl, &e);
	TEST1(2, AVLGetHeight(new_avl));
	AVLInsert(new_avl, &f);
	TEST1(2, AVLGetHeight(new_avl));
	AVLInsert(new_avl, &d);
	TEST1(2, AVLGetHeight(new_avl));
	
	AVLDestroy(new_avl);
}

void TestAVLRemove()
{
    avl_t *new_avl = AVLCreate(ComparFunc);
	int a = 10;
	int b = 6;
	int c = 13;
	int d = 12;
	int e = 8;
	int f = 20;
	int g = 5;
	int h = 7;
	int i = 9;
	int j = 11;
	 
    printf("TestAVLRemove\n");
	AVLInsert(new_avl, &a);
	AVLInsert(new_avl, &b);
	AVLInsert(new_avl, &c);
	AVLInsert(new_avl, &d);
	AVLInsert(new_avl, &e);
	AVLInsert(new_avl, &f);
	AVLInsert(new_avl, &g);
	AVLInsert(new_avl, &h);
	AVLInsert(new_avl, &i);
	TEST1(9, AVLSize(new_avl));
	TEST1(3, AVLGetHeight(new_avl));
	AVLRemove(new_avl, &a);

	printf("remove 10\n");
	AVLForEach(new_avl, PrintData, NULL);
    TEST1(8, AVLSize(new_avl));
    TEST1(4, AVLGetHeight(new_avl));
 	printf("height is %d size is %d\n",AVLGetHeight(new_avl), AVLSize(new_avl) );
    AVLRemove(new_avl, &d);
    printf("remove 12\n");
	AVLForEach(new_avl, PrintData, NULL);
	TEST1(7, AVLSize(new_avl));
	TEST1(3, AVLGetHeight(new_avl));
	AVLRemove(new_avl, &e);
	printf("remove 8\n");
	AVLForEach(new_avl, PrintData, NULL);
	TEST1(6, AVLSize(new_avl));
	TEST1(3, AVLGetHeight(new_avl));
	AVLRemove(new_avl, &g);
	printf("remove 5\n");
	AVLForEach(new_avl, PrintData, NULL);
	TEST1(5, AVLSize(new_avl));
	TEST1(3, AVLGetHeight(new_avl));
	
	AVLDestroy(new_avl);
}	
int ComparFunc(const void *new_data, const void *src_data) 
{
	 int result = *(int *)new_data - * (int *)src_data;
	 
	 if (result > 0)
	 {
	 	return 1;
	 }
	 
	 if (result < 0)
	 {
	 	return -1;
	 }
	 
	 return 0;
}

int PrintData(void *data, void *for_each_param)
{
	printf("%d\n", *(int *)data);

	UNUSED(for_each_param);
	
	return 0;
}
