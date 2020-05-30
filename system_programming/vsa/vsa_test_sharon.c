#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "vsa.h" 
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

void TestVSAInit();
void TestVSAAlloc();
void TestVSAFree();
void TestVSALargestChunk();

int main()
{
	TestVSAInit();
	TestVSAAlloc();
	TestVSAFree();
	TestVSALargestChunk();
	
	return 0;
}

void TestVSAInit()
{
	char *memory_pool = (char *)malloc(100);
	vsa_t *vsa = VSAInit(memory_pool, 100);
	/*char *memory_pool2 = (char *)malloc(0);
	TEST2(memory_pool2);*/	
	printf("TestVSAInit\n");
	TEST1((char *)vsa, memory_pool); 
	#ifdef NDEBUG
	TEST1(*(ssize_t *)(memory_pool), -68);
	TEST1(*(size_t *)(memory_pool + 8), 943579852543);
	TEST1(*(ssize_t *)(memory_pool + 84), 0);
	TEST1(*(size_t *)(memory_pool + 92), 943579852543);
	#endif
	 
	#ifndef NDEBUG
	TEST1(*(ssize_t *)(memory_pool), -84);
	TEST1(*(ssize_t *)(memory_pool + 92), 0);
	#endif
	 
	free(memory_pool);
}

void TestVSAAlloc()
{
	char *memory_pool = (char *)malloc(100);
	vsa_t *vsa = VSAInit(memory_pool, 100);
	vsa_t *header = VSAAlloc(vsa, 12);
	printf("TestVSAAlloc\n");
	#ifdef NDEBUG
	TEST1((char *)header, memory_pool + 16);
	TEST1(*(size_t *)(vsa), 16); 
	TEST1(*(ssize_t *)(memory_pool + 32), -36);
	VSAAlloc(vsa, 31);
	TEST1(*(ssize_t *)(memory_pool + 32), 36);
	TEST1(*(ssize_t *)(memory_pool + 40), 943579852543);
	header = VSAAlloc(vsa, 12); 
	TEST1(header ,NULL);
	TEST1(VSALargestChunk(vsa), 0);
	#endif
	
	#ifndef NDEBUG
	TEST1((char *)header, memory_pool + 8);
	TEST1(*(size_t *)(vsa), 16); 
	TEST1(*(ssize_t *)(memory_pool + 24), -60);
	VSAAlloc(vsa, 20);
	TEST1(*(ssize_t *)(memory_pool + 24), 24);
	TEST1(*(ssize_t *)(memory_pool + 56), -28);
	header = VSAAlloc(vsa, 44); 
	TEST1(header ,NULL);
	#endif
	free(memory_pool);
}

void TestVSAFree()
{
	char *memory_pool = (char *)malloc(160);
	vsa_t *vsa = VSAInit(memory_pool, 160);
	void *header1 = VSAAlloc(vsa, 12);
	void *header2 = VSAAlloc(vsa, 22);
	void *header3 = VSAAlloc(vsa, 3);
	void *header4 = VSAAlloc(vsa, 31);
	
	printf("TestVSAFree\n");
	#ifdef NDEBUG
	TEST1(VSALargestChunk(vsa), 0);
	VSAFree(header1);
	VSAFree(header2);
	VSAFree(header3);
	VSAFree(header4);
	TEST1(VSALargestChunk(vsa), 128);
	VSAFree(NULL);
	#endif
	
	#ifndef NDEBUG
	VSAAlloc(vsa, 7);
	VSAAlloc(vsa, 15);
	TEST1(*(ssize_t *)(memory_pool + 128), 16); /**/
	TEST1(*(ssize_t *)(memory_pool + 152), 0); /**/
	VSAFree(header1);
	VSAFree(header4);
	TEST1(VSALargestChunk(vsa), 32); /**/
	VSAFree(header2);
	TEST1(VSALargestChunk(vsa), 48); /**/
	TEST1(*(ssize_t *)(memory_pool + 56), 8); /**/
	TEST1(*(ssize_t *)(memory_pool), -16); /**/
	VSAAlloc(vsa, 48);
	TEST1(*(ssize_t *)(memory_pool), 48);
	TEST1(*(ssize_t *)(memory_pool + 56), 8);
	TEST1(VSALargestChunk(vsa), 32);
	#endif
	
	free(memory_pool);
}

void TestVSALargestChunk()
{
	char *memory_pool = (char *)malloc(160);
	vsa_t *vsa = VSAInit(memory_pool, 160);
	void *header1 = VSAAlloc(vsa, 12);
	void *header2 = VSAAlloc(vsa, 22);
	void *header3 = VSAAlloc(vsa, 3);
	void *header4 = VSAAlloc(vsa, 31);
	printf("TestVSALargestChunk\n");
	#ifdef NDEBUG
	TEST1(*(ssize_t *)(memory_pool), 16);
	TEST1(*(ssize_t *)(memory_pool + 32), 24);
	TEST1(*(ssize_t *)(memory_pool + 72), 8);
	TEST1(*(ssize_t *)(memory_pool + 96), 32);
	TEST1(*(ssize_t *)(memory_pool + 144), 0);
	VSAFree(header2);
	TEST1(*(ssize_t *)(memory_pool + 32), -24);
	TEST1(VSALargestChunk(vsa), 24);
	VSAFree(header3);
	TEST1(VSALargestChunk(vsa), 48);
	#endif
	
	#ifndef NDEBUG
	VSAAlloc(vsa, 32);
	TEST1(*(ssize_t *)(memory_pool), 16);
	TEST1(*(ssize_t *)(memory_pool + 24), 24);
	TEST1(*(ssize_t *)(memory_pool + 56), 8);
	TEST1(*(ssize_t *)(memory_pool + 72), 32);
	TEST1(*(ssize_t *)(memory_pool + 112), 32);
	TEST1(*(ssize_t *)(memory_pool + 152), 0);
	TEST1(memory_pool + 32, header2);
	VSAFree(header2);
	TEST1(VSALargestChunk(vsa), 24);
	VSAFree(header3);
	TEST1(VSALargestChunk(vsa), 40);
	VSAFree(header4);
	TEST1(VSALargestChunk(vsa), 80);
	VSAAlloc(vsa, 50);
	TEST1(*(ssize_t *)(memory_pool + 88), -16);
	TEST1(VSALargestChunk(vsa), 16);
	#endif

	free(memory_pool);
}
