
#include "recursion.h"
#include <stdio.h>
#include <string.h>


#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

void TestFibo(void);
void TestStrlen(void);
void TestStrcmp(void);
void TestStrcpy(void);
void TestStrcat(void);
void TestStrstr(void);
void TestFlip(void);
void TestSortStack(void);


int main()
{
	TestFibo();
	TestStrlen();
	TestStrcmp();
	TestStrcpy();
	TestStrcat();
	TestStrstr();
	TestFlip();
	TestSortStack();

	return 0;
}


int my_compare (void *data1, void *data2)
{
	return *(int*)data1 > *(int*)data2;
}

void TestSortStack(void)
{
	int arr1[10] = {4, 1, 8, 2, 11, 0, 3, 5, 9, 10};
	int i = 0;
	stack_t *mystack = (stack_t*)StackCreate(10,4);
	
	for (i = 0 ; i < 10 ; i++)
	{
		StackPush(mystack,&arr1[i]);
	}
	
	RecSortStack(mystack, 4, my_compare);
	for (i = 0 ; i < 10 ; i++)
	{
		printf("%d num is %d\n",i, *(int*)StackPeek(mystack));
		StackPop(mystack);
	}
	StackDestroy(mystack);
	
}


void TestStrstr(void)
{
	char hay[50] = "abcde";
	char needle[3] = "de";
	char *result = NULL;
	result = RecStrstr(hay, needle);
	PRINT_TEST(result == (hay +3), "strstr", 0);
}

void TestFlip(void)
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tFlip Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	PRINT_TEST(RecFlip(ptr_a) == ptr_d, "Flip return new head",0);
	PRINT_TEST((ptr_a->next) == NULL, "Flip old head points to null",0);
	PRINT_TEST((ptr_d->next) == ptr_c, "Flip new head points to one before",0);
	RecFlip(ptr_d);

	SListFreeAll(ptr_a);
}


void TestStrcat(void)
{
	char *str3 = "012345678";
	char str4[20] = "ffff";
	char str5[20] = "ffff";
	RecStrcat(str4, str3);
	RecStrcat(str5, str3);
	
	PRINT_TEST(RecStrcmp(str4, str5) == 0, "StrCat", 0);
}

void TestStrcpy(void)
{
	char *str1 = " ";
	char str2[1] = " ";
	char *str3 = "0123456789fds";
	char str4[13] = "";
	char *str5 = "ffff";
	char str6[4] = "";
	char *str7 = "dayum";
	char str8[6] = "";
	
	PRINT_TEST(RecStrcmp(RecStrcpy(str2, str1), str1) == 0, "Strcpy", 0);
	PRINT_TEST(RecStrcmp(RecStrcpy(str4, str3), str3) == 0, "Strcpy", 0);
	PRINT_TEST(RecStrcmp(RecStrcpy(str6, str5), str5) == 0, "Strcpy", 0);
	PRINT_TEST(RecStrcmp(RecStrcpy(str8, str7), str7) == 0, "Strcpy", 0);
}

void TestStrcmp(void)
{
	char *str1 = " ";
	char *str2 = " ";
	char *str3 = "0123456789fds";
	char *str4 = "0123456789fdsa";
	char *str5 = "ffff";
	char *str6 = "ffff";
	char *str7 = "dayum";
	char *str8 = "damnn";
	
	PRINT_TEST(RecStrcmp(str1, str2) == strcmp(str1, str2), "Strcmp", 0);

	PRINT_TEST(RecStrcmp(str3, str4) == strcmp(str3, str4), "Strcmp", 0);
	PRINT_TEST(RecStrcmp(str5, str6) == strcmp(str5, str6), "Strcmp", 0);
	PRINT_TEST(RecStrcmp(str7, str8) == strcmp(str7, str8), "Strcmp", 0);
}


void TestFibo(void)
{
/*
	PRINT_TEST(IterativeFibonacci(0) == 0, "Fibo", 0);
	PRINT_TEST(IterativeFibonacci(1) == 1, "Fibo", 1);
	PRINT_TEST(IterativeFibonacci(3) == 2, "Fibo", 3);
	PRINT_TEST(IterativeFibonacci(4) == 3, "Fibo", 4);
	PRINT_TEST(IterativeFibonacci(7) == 13, "Fibo", 7);
	PRINT_TEST(IterativeFibonacci(8) == 21, "Fibo", 8);	
	PRINT_TEST(IterativeFibonacci(31) == 1346269, "Fibo", 31);	
	PRINT_TEST(IterativeFibonacci(32) == 2178309, "Fibo", 32);	
	
	*/
	PRINT_TEST(RecursiveFibonacci(0) == 0, "Recursive Fibo", 0);
	PRINT_TEST(RecursiveFibonacci(1) == 1, "Recursive Fibo", 1);	
	PRINT_TEST(RecursiveFibonacci(3) == 2, "Recursive Fibo", 3);	
	PRINT_TEST(RecursiveFibonacci(4) == 3, "Recursive Fibo", 4);	
	PRINT_TEST(RecursiveFibonacci(7) == 13, "Recursive Fibo", 7);	
	PRINT_TEST(RecursiveFibonacci(8) == 21, "Recursive Fibo", 8);
	PRINT_TEST(RecursiveFibonacci(31) == 1346269, "Recursive Fibo", 31);
	PRINT_TEST(RecursiveFibonacci(32) == 2178309, "Recursive Fibo", 32);					
}

void TestStrlen(void)
{
	char *str1 = "maoz the king";
	char *str2 = "howmuchmore";
	char *str3 = "0123456789fds";
	char *str4 = "";
	char *str5 = "f";
	
	PRINT_TEST(strlen(str1) == RecStrlen(str1), "str", 1);
	PRINT_TEST(strlen(str2) == RecStrlen(str2), "str", 2);	
	PRINT_TEST(strlen(str3) == RecStrlen(str3), "str", 3);	
	PRINT_TEST(strlen(str4) == RecStrlen(str4), "str", 0);
	PRINT_TEST(strlen(str5) == RecStrlen(str5), "str", 0);

}
