
#include "vsa.h"
#include <stdio.h>
#include <stdlib.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 



void TESTVSAInit(void);
void TESTVSAAlloc(void);
void TESTVSAFree(void);

int main ()
{
	TESTVSAInit();
	TESTVSAAlloc();
	TESTVSAFree();
	return 0;
}

void TESTVSAInit(void)
{
	size_t memory_size = 150;
	void *pool = malloc(memory_size);
	vsa_t *my_vsa = NULL;

	my_vsa = VSAInit(pool, memory_size);

	printf("INIT TEST\n");
	#ifndef NDEBUG
	PRINT_TEST(*(ssize_t *)((char *)pool) == -134, "test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool + 134) == 0, "test", 1);
	#endif
	#ifdef NDEBUG
	PRINT_TEST(*(ssize_t *)((char *)pool) == -118, "test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool + 134) == 0, "test", 1);
	PRINT_TEST(*(size_t *)((char *)pool + 8) == magic_number, "magic_test", 1);
	PRINT_TEST(*(size_t *)((char *)pool + 142) == magic_number, "magic_test", 2);
	#endif
	free(my_vsa);

}

void TESTVSAAlloc(void)
{
	size_t memory_size = 150;
	size_t magic_number = 943579852543;
	void *pool = malloc(memory_size);
	vsa_t *my_vsa = NULL;
	void *header1 = NULL;
	void *header2 = NULL;
	void *header3 = NULL;
	void *header4 = NULL;
	
	printf("ALLOC TEST\n");
	my_vsa = VSAInit(pool, memory_size);
	header1 = VSAAlloc(pool, 0);
	PRINT_TEST(header1 == NULL, "0 alloc test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool) == -134, "0 alloc test", 2);
	header2 = VSAAlloc(pool, 20);
	PRINT_TEST(*(ssize_t *)((char *)pool) == 24, "first alloc test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool + 24 + 8) == -102, "first alloc test", 2);
	header3 = VSAAlloc(pool, 25);
	PRINT_TEST(*(ssize_t *)((char *)pool + 32) == 32, "second alloc test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool + 32 + 40) == -62, "second alloc test", 2);
	header4 = VSAAlloc(pool, 56);
	PRINT_TEST(*(ssize_t *)((char *)header4 - 8) == 62, "third alloc test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool + 142) == 0, "third alloc test", 2);
	
	free(my_vsa);
}

void TESTVSAFree(void)
{
	size_t memory_size = 150;
	size_t magic_number = 943579852543;
	void *pool = malloc(memory_size);
	vsa_t *my_vsa = NULL;
	void *header1 = NULL;
	void *header2 = NULL;
	void *header3 = NULL;
	void *header4 = NULL;
	void *header5 = NULL;
	void *header6 = NULL;
	printf("FREE TEST\n");
	my_vsa = VSAInit(pool, memory_size);
	header1 = VSAAlloc(pool, 0);
	VSAFree(header1);
	PRINT_TEST(header1 == NULL, "NULL free test", 1);
	header2 = VSAAlloc(pool, 20);
	VSAFree(header2);
	PRINT_TEST(*(ssize_t *)((char *)pool) == -24, "first free test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool + 24 + 8) == -102, "first free test", 2);
	header2 = VSAAlloc(pool, 20);
	header3 = VSAAlloc(pool, 25);
	header4 = VSAAlloc(pool, 30);
	VSAFree(header3);
	PRINT_TEST(*(ssize_t *)((char *)header3 - 8) == -32, "second free test", 1);
	PRINT_TEST(*(ssize_t *)((char *)header3 + 32 ) == 32, "second free test", 2);
	header3 = VSAAlloc(pool, 25);
	header5 = VSAAlloc(pool, 25);
	VSAFree(header3);
	PRINT_TEST(*(ssize_t *)((char *)header4 - 8) == 62, "third alloc test", 1);
	PRINT_TEST(*(ssize_t *)((char *)pool + 142) == 0, "third alloc test", 2);
	
	free(my_vsa);
}
