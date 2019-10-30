#include <stdio.h> /*prints*/
#include <string.h> /* memset */
#include <stdlib.h> /*mallocs and atoi*/
#include <math.h> /* POW */
#include <assert.h> /* assert */
#include "ws9.h"
#define STRING_SIZE 100


int main()
{
	TestMyMemset();
	TestMyMemcpy();
	TestMyMemmove();
	TestPrintLetters();
	TestItoaGeneral();
	TestAtoiGeneral();
	IsLittleEndian();

	return (0);
}

void TestMyMemset()
{
	char str1[] = {"This is an experiment and I hope it will succeed"};
	char str2[] = {"This is an experiment and I hope it will succeed"};
	long l1 = 999999999999;
	long l2 = 999999999999;
	int n1 = 999812999;
	int n2 = 999812999;
	
	printf("\n/*******************************************\n");
	printf("		Testing MyMemset				    	");
	printf("\n*******************************************/\n");
	
	MyMemset(str1 + 5, '*', 23);
	memset(str2 + 5, '*', 23);

	if (0 == memcmp(str1, str2, strlen(str2)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	MyMemset(str1 + 1, '9', 3);
	memset(str2 + 1, '9', 3);
	
	if (0 == memcmp(str1, str2, strlen(str2)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	MyMemset(str1 + 1, '9', 3);
	memset(str2 + 1, '9', 3);
	
	if (0 == memcmp(str1, str2, strlen(str2)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	MyMemset(&l1, '*', 4);
	memset(&l2, '*', 4);
	
	if (0 == memcmp(&l1, &l2, sizeof(long)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	MyMemset(&n1, '*', 2);
	memset(&n2, '*', 2);
	
	if (0 == memcmp(&n1, &n2, sizeof(int)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
}

void TestMyMemcpy()
{	
	char str_src[] = "Trying memcpy for the first time, cross your fingers";
	char memcpy_str_dst[STRING_SIZE] = {0};
	char mymemcpy_str_dst[STRING_SIZE] = {0};
	
	printf("\n/*******************************************\n");
	printf("		Testing MyMemcpy				    	");
	printf("\n*******************************************/\n");
		
	MyMemcpy(mymemcpy_str_dst, str_src + 2, 25);
	memcpy(memcpy_str_dst, str_src + 2, 25);
	
	if (0 == memcmp(mymemcpy_str_dst, memcpy_str_dst, 25))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
		
	MyMemcpy(mymemcpy_str_dst, str_src, 5);
	memcpy(memcpy_str_dst, str_src, 5);
	
	if (0 == memcmp(mymemcpy_str_dst, memcpy_str_dst, strlen(memcpy_str_dst)))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
}

void TestMyMemmove()
{
	char str1[] = {"This is an experiment and I hope it will succeed"};
	char str2[] = {"This is an experiment and I hope it will succeed"};
	int len = strlen(str1);
	
	printf("\n/*******************************************\n");
	printf("		Testing MyMemmove				    	");
	printf("\n*******************************************/\n");
	
	MyMemmove(str1 + 8, str1, 13);
	memmove(str2 + 8, str2, 13);
	
	if (0 == memcmp(str1, str2, len))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
	
	MyMemmove(str1 + 25, str1 + 30, 10);
	memmove(str2 + 25, str2 + 30, 10);
	
	if (0 == memcmp(str1, str2, len))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}

	MyMemmove(str1 + 1, str1, 4);
	memmove(str2 + 1, str2, 4);
	
	if (0 == memcmp(str1, str2, len))
	{
		printf("PASSED\n");
	}
	else
	{
		printf("FAILED\n");
	}
}

void TestItoaGeneral()
{
	int num_pos = 12056780;
	char str_pos[5][10] = {"55774314", "B7F8CC", "15LFL5", "BFU6C", "76F2K"};
	int num_neg = -12056780;
	char str_neg[5][10] = {"-55774314", "-B7F8CC", "-15LFL5", 
						   "-BFU6C", "-76F2K"};
	int base_array[] = {8,16,25,32,36};
	char *res = NULL;
	char* res_free = res;
	char tmp[999] = "";
	int i = 0;
	
	printf("\n/*******************************************\n");
	printf("		Testing MyItoa				    	");
	printf("\n*******************************************/\n");
	
	printf("Positive number\n");
	
	for (i = 0; i < 5; ++i)
	{
		res = MyItoa(num_pos, tmp, base_array[i]);
		if (0 == strcmp(res, str_pos[i]))
		{
			printf("PASSED for base %d\n", base_array[i]);
		}
		else
		{
			printf("FAILED for base %d\n", base_array[i]);
			printf("res = %s and expected = %s\n", res, str_pos[i]);
		}
		
	}
	
	printf("\nNegative number\n");
	
	for (i = 0; i < 5; ++i)
	{
		res = MyItoa(num_neg, tmp, base_array[i]);
		if (0 == strcmp(tmp, str_neg[i]))
		{
			printf("PASSED for base %d\n", base_array[i]);
		}
		else
		{
			printf("FAILED for base %d\n", base_array[i]);
			printf("res = %s and expected = %s\n", tmp, str_neg[i]);
		}
		

	}	
	free(res_free);
}

void TestAtoiGeneral()
{
	int i = 0;
	int base_array[] = {8,16,25,32,36};
	char num[5][15] = {"55774314", "B7F8CC", "15LFL5", "BFU6C", "76F2K"};
	int res = 0;
	int expres = 12056780;
	
	printf("\n/*******************************************\n");
	printf("		Testing AtoiGeneral				    	");
	printf("\n*******************************************/\n");
	
	for (i = 0; i < 5; ++i)
	{
		res = MyAtoi(num[i], base_array[i]);
		if (res == expres)
		{
			printf("PASSED for base %d\n", base_array[i]);
		}
		else
		{
			printf("FAILED for base %d\n", base_array[i]);
		}
	}
}

void TestPrintLetters()
{
	char *str1 = "654hgtf4k^&4@~";
	char *str2 = "654^&&&l";
	char *str3 = "1259*$%7757&";
	char expres[] = {'6', '4', '^'};
	int i = 0;
	
	printf("\n/*******************************************\n");
	printf("		Testing Print letters			    	");
	printf("\n*******************************************/\n");

	CheckThreeArrays(str1, str2, str3);
	
	for (i = 0; i < 3; ++i)
	{
		printf("%c, ", expres[i]);
	}
	
	printf("\n");
}


