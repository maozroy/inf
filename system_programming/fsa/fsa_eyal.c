/************************************************
* 	OL79 - 26/11/19								*
* free list										* 
* Author: Eyal Rabinovitz						*
* Reviewer: Maoz								*
************************************************/

#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "fsa.h"

#define WORD_SIZE sizeof(size_t)
#define BLOCK_HEADER_SIZE sizeof(fsa_block_header_t)
#define FSA_SIZE sizeof(fsa_t)
#define CHECK_PADDING(x) (WORD_SIZE == x) ? 0 : (x)

struct fsa
{
	size_t next_free;
	size_t block_size;
};

typedef struct block_header
{
	size_t offset;
} fsa_block_header_t;

static void SwapIMP(size_t *x, size_t *y);

fsa_t *FSAInit(void *memory_pool, size_t pool_size, size_t block_size)
{
	char *memory_pool_run = memory_pool;
	size_t padding = CHECK_PADDING(WORD_SIZE - (block_size % WORD_SIZE));
	size_t new_block_size = padding + block_size;
	size_t num_of_blocks = (pool_size - FSA_SIZE)
							/ (new_block_size + BLOCK_HEADER_SIZE);
	size_t i = 0;
	fsa_block_header_t *block_header = {0};
	fsa_t *fsa = (fsa_t *)memory_pool_run;

	assert(NULL != memory_pool);

	fsa -> next_free = FSA_SIZE + BLOCK_HEADER_SIZE;
	fsa -> block_size = block_size + padding;

	memory_pool_run = memory_pool_run + FSA_SIZE;

	for(i = 1; i < num_of_blocks; ++i)
	{
	
		block_header = (fsa_block_header_t *)memory_pool_run;
		block_header -> offset = (fsa -> next_free)
								 + (i * (new_block_size + BLOCK_HEADER_SIZE));

		memory_pool_run += BLOCK_HEADER_SIZE + new_block_size;
	}
	block_header = (fsa_block_header_t *)memory_pool_run;
	block_header -> offset = 0;

	return fsa;
}

size_t FSASuggestedSize(size_t num_of_blocks, size_t block_size)
{
	size_t padding = WORD_SIZE - (block_size % WORD_SIZE);
	size_t total_size = FSA_SIZE;
	total_size += num_of_blocks * (block_size + padding + BLOCK_HEADER_SIZE);

	return total_size;
}

void *FSAAlloc(fsa_t *fsa)
{
	fsa_block_header_t *alloc_block_header = {0};

	assert(NULL != fsa);

	alloc_block_header = (fsa_block_header_t *)((char *)fsa
											 + fsa -> next_free
											 - BLOCK_HEADER_SIZE);
	SwapIMP(&fsa -> next_free, &alloc_block_header -> offset);

	return ((void *) ((char *)alloc_block_header + sizeof(fsa -> block_size)));
}

void FSAFree(void *allocated_address)
{
	fsa_block_header_t *alloc_block_header = {0};
	fsa_t *fsa = {0};

	if(NULL != allocated_address)
	{
		alloc_block_header = (fsa_block_header_t *)((size_t)allocated_address
												 - BLOCK_HEADER_SIZE);
		fsa = (fsa_t *)((size_t)allocated_address
					 - alloc_block_header -> offset);

		SwapIMP(&fsa -> next_free, &alloc_block_header -> offset);		
	}
}

size_t FSACountFree(fsa_t *fsa)
{
	size_t next_free = 0;
	size_t counter = 0;
	fsa_block_header_t *header = {0};
	
	assert(fsa);
	
	next_free = fsa -> next_free;	
	
	while (0 != next_free)
	{
		++counter;
		header = (fsa_block_header_t *)
				 ((char *)fsa + next_free - BLOCK_HEADER_SIZE);
		next_free = header -> offset;
	}	

	return counter;
}

static void SwapIMP(size_t *x, size_t *y)
{
	size_t temp = *x;

	*x = *y;
	*y = temp;
}
