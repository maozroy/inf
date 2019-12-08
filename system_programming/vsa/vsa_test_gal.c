/****************************************************************************
*                                                                           *
*			                     Gal Salemon		                        *
*                              OL79 - 1/12/19                               *
*                                                                           *
*                                   vsa                                     *
*                             Reviewer: Yonatan                             *
*****************************************************************************/
#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "vsa.h"
#include "myutils.h"

#define BLOCK_SIZE 5
#define MEMORY_POOL 100
#define NUM_OF_BLOCKS 10
#define MAGIC_NUMBER 943579852543

void TestFlowWithoutDebug()
{
    vsa_t *vsa = NULL;
    char *memory_pool = (char*)malloc(MEMORY_POOL);
    size_t block_size = 8;
    char *memory_location = NULL;
    char *memory_location1 = NULL;
    char *memory_location2 = NULL;
    
    vsa = VSAInit(memory_pool, MEMORY_POOL);
    TEST("Init", *(memory_pool) == -84);
    TEST("Init", 0 == *(char*)(memory_pool + MEMORY_POOL - 8));
	TEST("Largest Chunk", VSALargestChunk(vsa) == 84);
  
    memory_location = VSAAlloc(vsa, block_size);
    
    /*printf("memory_location: %p\n", memory_location);*/
    TEST("Alloc", *(memory_pool + 16) == -68);
    TEST("Alloc", (memory_pool + 8) == memory_location);
    
    block_size = 16;
    memory_location = VSAAlloc(vsa, block_size);
	TEST("Alloc", (memory_pool + 24) == memory_location);
	TEST("Alloc", *(memory_pool + 16) == 16);
	TEST("Alloc", *(memory_pool + 40) == -44);
	
    /*
    printf("%d\n", *(memory_pool + 40));  
    printf("%d\n", *(memory_pool + 16));
    */    
     
    VSAFree(memory_location);
    TEST("Free", *(memory_pool + 16) == -16);
    
    memory_location = VSAAlloc(vsa, block_size);
	TEST("Alloc", (memory_pool + 24) == memory_location);
	TEST("Alloc", *(memory_pool + 16) == 16);
	TEST("Alloc", *(memory_pool + 40) == -44);

	block_size = 35;
	memory_location1 = VSAAlloc(vsa, block_size);
	TEST("Alloc", (memory_pool + 48) == memory_location1);
	printf("%d\n",*(memory_pool + 40));
	TEST("Alloc", *(memory_pool + 40) == 44);
	TEST("Largest Chunk", VSALargestChunk(vsa) == 0);
	
	VSAFree(memory_location);
    TEST("Alloc", *(memory_pool + 16) == -16);
    TEST("Largest Chunk", VSALargestChunk(vsa) == 16);
   
	free(memory_pool);
}

void TestFlowWithDebug()
{
	vsa_t *vsa = NULL;
    char *memory_pool = (char*)malloc(MEMORY_POOL);
    size_t block_size = 8;
    char *memory_location = NULL;
    char *memory_location1 = NULL;
    char *memory_location2 = NULL;
    char *memory_location3 = NULL;
    
    vsa = VSAInit(memory_pool, MEMORY_POOL);
    TEST("Init", *(memory_pool) == -68);
    TEST("Init", 0 == *(char*)(memory_pool + MEMORY_POOL - 16));
	TEST("Largest Chunk", VSALargestChunk(vsa) == 68);
    
    memory_location = VSAAlloc(vsa, block_size);
    
    /**/
    printf("memory_location: %d\n", *(size_t*)(memory_pool));
    
    TEST("Init", *(memory_pool) == 8);
    TEST("Init", *(size_t*)(memory_pool + 8) == MAGIC_NUMBER);
    TEST("Alloc", *(memory_pool + 24) == -44);
    TEST("Alloc", (memory_pool + 16) == memory_location);
    
    block_size = 16;
    memory_location1 = VSAAlloc(vsa, block_size);
	TEST("Alloc", (memory_pool + 40) == memory_location1);
	TEST("Alloc", *(memory_pool + 24) == 16);
	TEST("Alloc", *(memory_pool + 56) == -12);
	
	VSAFree(memory_location1);
    TEST("Free", *(memory_pool + 24) == -16);
    TEST("Free", *(memory_pool + 56) == -12);
    
    TEST("Largest Chunk", VSALargestChunk(vsa) == 44);
    
    block_size = 12;
    memory_location1 = VSAAlloc(vsa, block_size);
	TEST("Alloc", (memory_pool + 40) == memory_location1);
	TEST("Alloc", *(memory_pool + 24) == 16);
	
	block_size = 8;
	memory_location2 = VSAAlloc(vsa, block_size);
	TEST("Alloc", (memory_pool + 72) == memory_location2);
	TEST("Alloc", *(memory_pool + 56) == 12);
	TEST("Largest Chunk", VSALargestChunk(vsa) == 0);
	
	memory_location3 = VSAAlloc(vsa, block_size);
	TEST("Alloc", NULL == memory_location3);
	
	VSAFree(memory_location1);
	TEST("Largest Chunk", VSALargestChunk(vsa) == 16);
	VSAFree(memory_location);
	TEST("Largest Chunk", VSALargestChunk(vsa) == 40);
	VSAFree(memory_location2);
	TEST("Largest Chunk", VSALargestChunk(vsa) == 68);
	
	block_size = 0;
	memory_location3 = VSAAlloc(vsa, block_size);
	TEST("Alloc 0 bytes", NULL == memory_location3);
	
	free(memory_pool);
}

int main()
{
    #ifndef NDEBUG
	TestFlowWithoutDebug();
	#endif
	
	#ifdef NDEBUG
	TestFlowWithDebug();
	#endif
	
    return 0;
}


