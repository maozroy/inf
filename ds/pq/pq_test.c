#include "pq.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

int MyIntCompare (const void *new_data, const void *src_data, void *param);
void TestPQCreate(void);
void TestPQInsert(void);


int main()
{
	TestPQCreate();
	TestPQInsert();
	return 0;
}

void TestPQCreate(void)
{
	p_queue_t *my_pq = PQCreate(NULL, MyIntCompare);
	PQDestroy(my_pq);
}


int MyIntCompare (const void *new_data, const void *src_data, void *param)
{
	int new = *(int*)new_data;
	int src = *(int*)src_data;
	
	int result = 0;
	param = param;
	
	if (new >= src*10)
	{
		result = 1;	
	}
	else if (new <= src*10)
	{
		result = -1;
	}
	return result;
	
}

void TestPQInsert(void)
{
	int a = 1;
	int b = 22;
	int c = 5;
	int d = 8721;
	int e = 124;
	int f = 896;
	
	p_queue_t *my_pq = PQCreate(NULL, MyIntCompare);
	PQEnqueue(my_pq, &a);
	PQEnqueue(my_pq, &b);
	PQEnqueue(my_pq, &c);	
	PQEnqueue(my_pq, &d);	
	PQEnqueue(my_pq, &e);	
	PQEnqueue(my_pq, &f);	
	
	printf("%ld",PQSize(my_pq));
	
	
	PQDestroy(my_pq);
}















