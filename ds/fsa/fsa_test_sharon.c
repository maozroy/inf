#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "fsa.h" 
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

void TestFSAInit();
void TestFSASuggestedSize();
void TestFSAAlloc();
void TestFSAFree();
void TestFSACountFree();

int main()
{
	TestFSAInit();
	TestFSASuggestedSize();
	TestFSAAlloc();
	TestFSAFree();
	TestFSACountFree();
	
	return 0;
}

void TestFSAInit()
{
	size_t *memory_pool = (size_t *)malloc(176);
	fsa_t *fsa = FSAInit(memory_pool, 176, 5);
	
	printf("TestFSAInit\n");
	TEST2(fsa);
	TEST1(*(memory_pool), 16);
	TEST1(*(memory_pool + 1), 16);
	TEST1(*(memory_pool + 2), 32);
	TEST1(*(memory_pool + 4), 48);
	TEST1(*(memory_pool + 6), 64);
	TEST1(*(memory_pool + 8), 80);
	TEST1(*(memory_pool + 10), 96);
	TEST1(*(memory_pool + 12), 112);
	TEST1(*(memory_pool + 14), 128);
	TEST1(*(memory_pool + 16), 144);
	TEST1(*(memory_pool + 18), 160);
	TEST1(*(memory_pool + 20), 0);
	
	free(memory_pool);
}

void TestFSASuggestedSize()
{
 	size_t suggested_size = FSASuggestedSize(10, 5);
 	
 	printf("TestFSASuggestedSize\n");
 	TEST1(suggested_size, 176);
}

void TestFSAAlloc()
{
	size_t *memory_pool = (size_t *)malloc(176);
	void *allocated_address = NULL;
	fsa_t *fsa = FSAInit(memory_pool, 176, 5);
	allocated_address = FSAAlloc(fsa);
	printf("TestFSAAlloc\n");
	TEST1(*(memory_pool), 32);
	TEST1(*(memory_pool + 2), 16);
	TEST1(allocated_address, memory_pool + 3);
	
	free(memory_pool);
}

void TestFSAFree()
{
	size_t *memory_pool = (size_t *)malloc(176);
	void *block1_address = NULL;
	fsa_t *fsa = FSAInit(memory_pool, 176, 5);
	block1_address = FSAAlloc(fsa);
	FSAFree(NULL);
	FSAAlloc(fsa);
	FSAFree(block1_address);
	printf("TestFSAFree\n");
	TEST1(*(memory_pool), 16);
	TEST1(*(memory_pool + 2), 48);
	TEST1(*(memory_pool + 4), 32);

	free(memory_pool);	
}

void TestFSACountFree()
{
	size_t *memory_pool = (size_t *)malloc(176);
	void *block1_address = NULL;
	void *block2_address = NULL;
	void *block3_address = NULL;
	void *block4_address = NULL;
	void *block5_address = NULL;
	void *block6_address = NULL;
	void *block7_address = NULL;
	void *block8_address = NULL;
	void *block9_address = NULL;
	void *block10_address = NULL;
	fsa_t *fsa = FSAInit(memory_pool, 176, 5);
	block1_address = FSAAlloc(fsa);
	block2_address = FSAAlloc(fsa);
	block3_address = FSAAlloc(fsa);
	block4_address = FSAAlloc(fsa);
	block5_address = FSAAlloc(fsa);
	block6_address = FSAAlloc(fsa);
	block7_address = FSAAlloc(fsa);
	block8_address = FSAAlloc(fsa);
	block9_address = FSAAlloc(fsa);
	block10_address = FSAAlloc(fsa);
	printf("TestFSACountFree\n");
	TEST1(0, FSACountFree(fsa));
	FSAFree(block2_address);
	TEST1(1, FSACountFree(fsa));
	FSAFree(block1_address);
	TEST1(2, FSACountFree(fsa));
	FSAFree(block3_address);
	TEST1(3, FSACountFree(fsa));
	FSAFree(block10_address);
	TEST1(4, FSACountFree(fsa));
	FSAFree(block8_address);
	TEST1(5, FSACountFree(fsa));
	FSAFree(block6_address);
	TEST1(6, FSACountFree(fsa));
	FSAFree(block5_address);
	TEST1(7, FSACountFree(fsa));
	FSAFree(block4_address);
	TEST1(8, FSACountFree(fsa));
	FSAFree(block9_address);
	TEST1(9, FSACountFree(fsa));
	FSAFree(block7_address);
	TEST1(10, FSACountFree(fsa));
	
	free(memory_pool);	
}
