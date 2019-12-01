
#include "fsa.h"
#include <stdio.h>
#include <stdlib.h>

#define PRINT_TEST(name, num, test) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

typedef struct block_header
{
	size_t offset;
} fsa_block_header_t;

struct fsa
{
	size_t next_free;
	size_t block_size;
};


void TestFSAInit();
void TestFSAAlloc();
void TestFSACountFree();


int main()
{
	TestFSAInit();
	TestFSAAlloc();
	TestFSACountFree();

	return 0;
}

void TestFSACountFree()
{
	fsa_t * new_fsa = NULL;
	void * ptr = NULL;
	new_fsa = malloc(FSASuggestedSize(13, 4));
	new_fsa = FSAInit(new_fsa, FSASuggestedSize(13, 4), 4);
	PRINT_TEST("Count test 13", 0, (FSACountFree(new_fsa)) == 13 ); 
	FSAAlloc(new_fsa);
	PRINT_TEST("Count test 12", 1, (FSACountFree(new_fsa)) == 12 ); 
	FSAAlloc(new_fsa);
	FSAFree(FSAAlloc(new_fsa));
	FSAAlloc(new_fsa);	
	FSAAlloc(new_fsa);
	PRINT_TEST("Count 9", 2, (FSACountFree(new_fsa)) == 9 ); 
	FSAAlloc(new_fsa);
	FSAAlloc(new_fsa);
	FSAFree(FSAAlloc(new_fsa));
	FSAAlloc(new_fsa);
	FSAAlloc(new_fsa);
	FSAAlloc(new_fsa);
	ptr = FSAAlloc(new_fsa);
	PRINT_TEST("Count 9", 2, (FSACountFree(new_fsa)) == 3 ); 
	FSAAlloc(new_fsa);
	FSAAlloc(new_fsa);
	FSAAlloc(new_fsa);
	PRINT_TEST("Count empty", 1, 
	(FSACountFree(new_fsa)) == 0); 	
	

	free(new_fsa);
	
}

void TestFSAInit()
{
	fsa_t * new_fsa = NULL;
	new_fsa = malloc(FSASuggestedSize(10, 4));
	new_fsa = FSAInit(new_fsa, 176, 4);
	
	PRINT_TEST("First next", 1, 
				((size_t)((fsa_t *)(new_fsa->next_free)) == 24 )); 
	PRINT_TEST("First offset", 1, 
				(((fsa_block_header_t *)((char*)new_fsa + sizeof(fsa_t)))->offset == 40 )); 
	PRINT_TEST("Last block offset", 2, 
				(((fsa_block_header_t *)((char*)new_fsa + 160))->offset == 0 )); 	
				printf("counter: %ld should be 10\n",FSACountFree(new_fsa));
	free(new_fsa);
	
}




void TestFSAAlloc()
{
	fsa_t * new_fsa = NULL;
	new_fsa = malloc(FSASuggestedSize(10, 4));
	new_fsa = FSAInit(new_fsa, 176, 4);
	FSAAlloc(new_fsa);
	PRINT_TEST("First next", 1, 
				((size_t)((fsa_t *)(new_fsa->next_free)) == 40 )); 
	PRINT_TEST("First offset", 1, 
				(((fsa_block_header_t *)((char*)new_fsa + sizeof(fsa_t)))->offset == 24 )); 
	PRINT_TEST("Last block offset", 2, 
				(((fsa_block_header_t *)((char*)new_fsa  + 160))->offset == 0 )); 	
					FSAAlloc(new_fsa);
	free(new_fsa);
	
}
