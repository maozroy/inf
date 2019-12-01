 /********************************************
*	Author : Maoz Roytman
*	Reviewer : 	Gal Salemon
*	vsa					
*	Date:2/12/19	                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/

#include "vsa.h"
#define HEADER_SIZE sizeof(vsa_block_header_t)
#define WORD_LENGTH sizeof(size_t)
#define BLOCK vsa_block_header_t

typedef struct block_header
{
	ssize_t block_size;
	#ifdef DEBUG
    size_t magic_number;
    #endif
}vsa_block_header_t;

size_t magic_number = 943579852543;

static size_t WordAlignedIMP (size_t block_size);

static BLOCK *FindClosestFreeSpaceIMP(vsa_t *vsa, size_t block_size);

/*sums space from block until next header, until block_size*/
static size_t SumFreeSpaceIMP(BLOCK *block, size_t needed_space);

static BLOCK *NextBlockIMP(BLOCK *block);


vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	vsa_t *vsa_begin = NULL;
	BLOCK *end_header = NULL;
	
	assert(pool_size);
	
	vsa_begin = memory_pool;
	end_header = (BLOCK *)((char *)memory_pool - HEADER_SIZE);
	
	vsa_begin->block_size = - pool_size + 2 * HEADER_SIZE;
	end_header->block_size = 0;
	
	#ifdef DEBUG
	vsa_begin->magic_number = magic_number;
	end_header->magic_number = magic_number;
    #endif
	
	return vsa_begin;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	BLOCK * alloc_header = NULL;
	BLOCK * next_header = NULL;
	size_t next_header_space = 0;
	block_size = WordAlignedIMP(block_size);
	
	alloc_header = FindClosestFreeSpaceIMP(vsa, block_size);
    
    next_header = (BLOCK *)((char *)alloc_header + block_size + HEADER_SIZE);
    next_header_space = SumFreeSpaceIMP(alloc_header, block_size + HEADER_SIZE);
    if (next_header_space < HEADER_SIZE)
    {
	 	/*don't write next header*/
	 	/*add to block size the difference*/
	}
    next_header->block_size = SumFreeSpaceIMP(alloc_header, block_size + HEADER_SIZE);
    
    alloc_header->block_size = block_size;

	#ifdef DEBUG
	next_header->magic_number = magic_number;
    #endif
    
    return (void *)((char *)alloc_header + HEADER_SIZE);
}

static BLOCK *FindClosestFreeSpaceIMP(vsa_t *vsa, size_t block_size)
{
	

}

static size_t SumFreeSpaceIMP(BLOCK *block, size_t needed_space)
{
	size_t free_space_counter = 0;
	
	while ((free_space_counter < needed_space) || (block->block_size < 0))
	{
		free_space_counter += -1 * (block->block_size);
		block = NextBlockIMP(block);	
	}
	
	return free_space_counter;

}

static BLOCK *NextBlockIMP(BLOCK *block)
{
	return (BLOCK *)((char *)block + block->block_size);
}

static size_t WordAlignedIMP (size_t block_size)
{
	if (((block_size+HEADER_SIZE) % WORD_LENGTH) != 0)
	{
		return ((block_size + HEADER_SIZE) / 
				WORD_LENGTH ) * WORD_LENGTH + WORD_LENGTH;
	}
	
	return (block_size + HEADER_SIZE) * WORD_LENGTH;
}
