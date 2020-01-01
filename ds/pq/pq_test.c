#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /*strcmp*/

#include "pq.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

void TestPQCreate();
void TestPQEnqueue();
void TestPQDequeue();
void TestPQPeek();
void TestPQIsEmpty();
void TestPQSize();
void TestPQClear();
void TestPQErase();

int CompareFunc(const void *new_data, const void *src_data, void *param);
int IsMatch(const void *new_data, const void *param);

int main()
{
	/*TestPQCreate();
	TestPQEnqueue();
	TestPQDequeue();
	TestPQPeek();

	TestPQSize();
	TestPQClear();
	TestPQErase();*/
		TestPQIsEmpty();

	return 0;
}

void TestPQCreate()
{

 	p_queue_t *pq = PQCreate(NULL, CompareFunc);
 	printf("TestPQCreate\n");
 	TEST2(pq);
 	TEST1(PQIsEmpty(pq), 1);
	TEST1(PQSize(pq), 0);
 	PQDestroy(pq);
}

void TestPQEnqueue()
{
	int param = 1;
 	p_queue_t *pq = PQCreate(&param, CompareFunc);
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int data4 = 4;
 	printf("TestPQEnqueue\n");
	TEST1(0, PQEnqueue(pq, &data1));
	TEST1(*(int*)PQPeek(pq), 1);
	TEST1(0, PQEnqueue(pq, &data3));
	TEST1(*(int*)PQPeek(pq), 3);
	TEST1(0, PQEnqueue(pq, &data4));
	TEST1(*(int*)PQPeek(pq), 4);
	TEST1(0, PQEnqueue(pq, &data2));
	TEST1(*(int*)PQPeek(pq), 4);
	PQDestroy(pq);   
}

void TestPQDequeue()
{
 	int param = 1;
 	p_queue_t *pq = PQCreate(&param, CompareFunc);
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int data4 = 4;
 	printf("TestPQDequeue\n");
 	PQEnqueue(pq, &data1);
	PQEnqueue(pq, &data3);
	PQEnqueue(pq, &data4);
	PQEnqueue(pq, &data2);
	PQDequeue(pq);
	TEST1(*(int*)PQPeek(pq), 3);
	PQDequeue(pq);
	TEST1(*(int*)PQPeek(pq), 2);
	PQDequeue(pq);
	TEST1(*(int*)PQPeek(pq), 1);
	PQDequeue(pq);
	TEST1(PQIsEmpty(pq), 1);
 	PQDestroy(pq);
}

void TestPQPeek()
{
 	int param = 1;
 	p_queue_t *pq = PQCreate(&param, CompareFunc);
	int data1 = 43;
	int data2 = 50;
	int data3 = 12;
	int data4 = 4;
 	printf("TestPQPeek\n");
 	PQEnqueue(pq, &data1);
	PQEnqueue(pq, &data3);
	PQEnqueue(pq, &data4);
	PQEnqueue(pq, &data2);
	PQDequeue(pq);
	TEST1(*(int*)PQPeek(pq), 43);
	PQDequeue(pq);
	TEST1(*(int*)PQPeek(pq), 12);
	PQDequeue(pq);
	TEST1(*(int*)PQPeek(pq), 4);
	PQDequeue(pq);
	TEST1(PQIsEmpty(pq), 1);
 	PQDestroy(pq);
}
 	
void TestPQIsEmpty()
{
	int param = 1;
 	p_queue_t *pq = PQCreate(&param, CompareFunc);
 	int data1 = 43;
 	printf("TestPQIsEmpty\n");
 	TEST1(PQIsEmpty(pq), 1);
 	PQEnqueue(pq, &data1);
	TEST1(PQIsEmpty(pq), 0);
	/*PQDequeue(pq);*/
	PQErase(&data1, pq, IsMatch);
	TEST1(PQIsEmpty(pq), 1);
	PQDestroy(pq);	
}

void TestPQSize()
{
 	int param = 1;
 	p_queue_t *pq = PQCreate(&param, CompareFunc);
	int data1 = 43;
	int data2 = 50;
	printf("TestPQSize\n");
	TEST1(PQSize(pq), 0);
	PQEnqueue(pq, &data1);
	PQEnqueue(pq, &data2);
	TEST1(PQSize(pq), 2);
	PQDequeue(pq);
	TEST1(PQSize(pq), 1);
	PQDequeue(pq);
	TEST1(PQSize(pq), 0);
	PQDestroy(pq);
}

void TestPQClear()
{
  	int param = 1;
 	p_queue_t *pq = PQCreate(&param, CompareFunc);
	int data1 = 43;
	int data2 = 50;
 	printf("TestPQClear\n");
 	PQEnqueue(pq, &data1);
	PQEnqueue(pq, &data2);
 	TEST1(PQSize(pq), 2);
 	PQClear(pq);
 	TEST1(PQSize(pq), 0);
 	TEST1(PQIsEmpty(pq), 1);
	PQDestroy(pq);
}

void TestPQErase()
{
  	int param = 1;
 	p_queue_t *pq = PQCreate(&param, CompareFunc);
 	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int data4 = 4;
 	PQEnqueue(pq, &data1);
	PQEnqueue(pq, &data3);
	PQEnqueue(pq, &data4);
	PQEnqueue(pq, &data2);
 	printf("TestPQErase\n");
 	TEST1(PQSize(pq), 4);
 	PQErase(&data3, pq, IsMatch);
 	TEST1(PQSize(pq), 3);
 	PQDestroy(pq);

}

int CompareFunc(const void *new_data, const void *src_data, void *param)
{	
	if (1 == *(int*) param)
	{
		if (*(int*)new_data > *(int*)src_data)
		{
			return 1;
		}
		
		else if (*(int*)new_data < *(int*)src_data)
		{
			return -1;
		}
	
		return 0;
	}
	/*
	if (2 == *(int*)param)
	{
		if (*(int*)new_data > *(int*)src_data)
		{
			return 1;
		}
	
		return 0;
	}*/
	
	return 0;
}

int IsMatch(const void *new_data, const void *param)
{
	return (*(int*)new_data == *(int*)param);
}

