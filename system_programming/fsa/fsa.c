 /********************************************
*	Author : Maoz Roytman
*	Reviewer : Tamir Dayan
*	fsa					
*	Date: Nov 27 2019		                
*																				
********************************************/

#include <assert.h> /*assert*/

#include "fsa.h"
#define WORD_LENGTH sizeof(size_t)
#define BLOCK_POINTER fsa_block_header_t *

/*managing struct*/
struct fsa
{
	size_t next_free;
	size_t block_size;
};

/*block header struct*/
typedef struct block_header
{
	size_t offset;
} fsa_block_header_t;

static size_t WordAlignedIMP (size_t block_size);
static void CreateHeaderIMP(BLOCK_POINTER block, size_t offset);
static void SwapSizetIMP(size_t *x, size_t *y);

fsa_t *FSAInit(void *memory_pool, size_t pool_size, size_t block_size)
{
	size_t num_of_blocks = 0;
	size_t i = 0;
	fsa_t *new_fsa = NULL;
	char *pool_runner = NULL;
	
	assert(memory_pool);
	
	new_fsa = memory_pool;
	new_fsa->block_size = WordAlignedIMP(block_size);
	new_fsa->next_free = sizeof(fsa_t);
	pool_runner = (char *)memory_pool;
	num_of_blocks = (pool_size - sizeof(fsa_t)) / new_fsa->block_size;
	pool_runner += new_fsa->next_free;
	
	for (i = 0 ; i < num_of_blocks ; ++i)
	{
		CreateHeaderIMP((BLOCK_POINTER)pool_runner,
						 new_fsa->next_free + (i + 1) * (new_fsa->block_size));
 		pool_runner = pool_runner + (new_fsa->block_size);
	}
	CreateHeaderIMP((BLOCK_POINTER) (pool_runner - new_fsa->block_size), 0);
	
	return new_fsa;
}

size_t FSASuggestedSize(size_t num_of_blocks, size_t block_size)
{
	block_size = WordAlignedIMP(block_size);
	
	return ((block_size * num_of_blocks) + sizeof(fsa_t));
}

void *FSAAlloc(fsa_t *fsa)
{
	void *allocated = NULL;
	
	assert(fsa);
	
	allocated = (char *)fsa + fsa->next_free + sizeof(fsa_block_header_t);
	SwapSizetIMP(&((BLOCK_POINTER)((char *)allocated - 
				sizeof(fsa_block_header_t)))->offset, 
				&fsa->next_free);

	return allocated;
}

size_t FSACountFree(fsa_t *fsa)
{
	size_t counter = 1;
	char *runner = (char *)fsa;
	BLOCK_POINTER block = (BLOCK_POINTER) ((char *)fsa + fsa->next_free);
	
	if (fsa->next_free == 0)
	{
		counter = 0;
	}
	else
	{
		while (block->offset != 0)
		{	
			runner = (char*)fsa + block->offset;
			block = (BLOCK_POINTER)runner;
			++counter;
		}
	}
	
	return counter;
}

void FSAFree(void *allocated_address)
{
	BLOCK_POINTER my_header = NULL;
	fsa_t *fsa = NULL;

	if (NULL == allocated_address)	
	{
		return;
	}
	
	my_header = (BLOCK_POINTER) ((char *)allocated_address - 
				sizeof(fsa_block_header_t));
	fsa = (fsa_t *)((char *)allocated_address - 
			sizeof(fsa_block_header_t) - my_header->offset);									

	SwapSizetIMP(&fsa->next_free, &my_header->offset);
}

static void SwapSizetIMP(size_t *x, size_t *y)
{
	size_t temp = *x;
	
	*x = *y;
	*y = temp;
}

static void CreateHeaderIMP(BLOCK_POINTER block, size_t offset)
{
	block->offset = offset;
}

static size_t WordAlignedIMP (size_t block_size)
{
	if (((block_size+sizeof(fsa_block_header_t)) % WORD_LENGTH) != 0)
	{
		return ((block_size + sizeof(fsa_block_header_t)) / 
				WORD_LENGTH ) * WORD_LENGTH + WORD_LENGTH;
	}
	
	return (block_size + sizeof(fsa_block_header_t)) * WORD_LENGTH;
}
