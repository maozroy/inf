 /********************************************
*	Author : Maoz Roytman
*	Reviewer : 	Gal Salemon
*	vsa					
*	Date:2/12/19	                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <stdio.h>


#include "vsa.h"
#define HEADER_SIZE sizeof(vsa_block_header_t)
#define WORD_LENGTH sizeof(size_t)
#define BLOCK vsa_block_header_t


typedef struct block_header
{
	ssize_t block_size;
	#ifdef NDEBUG
    size_t magic_number;
    #endif
}vsa_block_header_t;

size_t magic_number = 943579852543;

static size_t WordAlignedIMP (size_t block_size);

static BLOCK *FindClosestFreeSpaceIMP(vsa_t *vsa, ssize_t needed_space);

/*sums space from block until next header, until block_size*/
static size_t SumFreeSpaceIMP(BLOCK *block, size_t needed_space);

static BLOCK *NextBlockIMP(BLOCK *block);


vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	vsa_t *vsa_begin = NULL;
	BLOCK *end_header = NULL;
	
	assert(pool_size);
	
	vsa_begin = memory_pool;
	end_header = (BLOCK *)((char *)memory_pool + pool_size - HEADER_SIZE);
	
	vsa_begin->block_size = - pool_size + 2 * HEADER_SIZE;
	end_header->block_size = 0;
	
	#ifdef NDEBUG
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
	if (alloc_header == NULL)
	{
		return NULL;
	}
	if (alloc_header->block_size == 0 || block_size == 0)
	{
		return NULL;
	}
    
    next_header = (BLOCK *)((char *)alloc_header + block_size + HEADER_SIZE);
    next_header_space = SumFreeSpaceIMP(alloc_header, block_size);
    
    if (next_header_space <= (HEADER_SIZE*2 + block_size))
    {
 	   alloc_header->block_size = next_header_space;
	}
	else
	{
	    alloc_header->block_size =  block_size;
		next_header->block_size = -(next_header_space - 
									block_size - HEADER_SIZE);
		#ifdef NDEBUG
		next_header->magic_number = magic_number;
		#endif
	}
    
    return (void *)((char *)alloc_header + HEADER_SIZE);
}

void VSAFree(void *allocated_address)
{
	BLOCK *block = (BLOCK *)((char *)allocated_address - HEADER_SIZE);
	if (allocated_address == NULL)
	{
		return;
	}
	#ifdef NDEBUG
	if (block->magic_number != magic_number)
	{
		return;
	}
	#endif
	
	block->block_size *= -1;
}


size_t VSALargestChunk(vsa_t *vsa)
{
	ssize_t counter = 0;
	ssize_t max_counter = 0;
	BLOCK *runner = NULL;
	
	runner = vsa;
	
	while (runner->block_size != 0)
	{
		if (runner->block_size < 0)
		{
			counter += abs(runner->block_size) + HEADER_SIZE;
		}
		else
		{
			if (counter > max_counter)
			{
				max_counter = counter;
			}
			counter = 0;
		}
		runner = NextBlockIMP(runner);
	}
	if (counter > max_counter)
	{
		max_counter = counter;
	}
	max_counter -= HEADER_SIZE;
	if (max_counter < 0)
	{
		max_counter = 0;
	}
	return max_counter;
}


static BLOCK *FindClosestFreeSpaceIMP(vsa_t *vsa, ssize_t needed_space)
{
	ssize_t counter = 0;
	ssize_t max_counter = 0;
	BLOCK *out_runner = NULL;
	BLOCK *inner_runner = NULL;
	
	out_runner = vsa;
	while (out_runner->block_size != 0 && max_counter < needed_space)
	{
		inner_runner = out_runner;
		
		while ((inner_runner->block_size < 0) )
		{
			counter += abs(inner_runner->block_size) + HEADER_SIZE;
			inner_runner = NextBlockIMP(inner_runner);	
		}
		if (counter > max_counter)
		{
			max_counter = counter;
		}
		
		if (max_counter >= needed_space)
		{
			break;
		}
		
		out_runner = NextBlockIMP(out_runner);
		counter = 0;
	}
	
	max_counter -= HEADER_SIZE;
	if (max_counter < needed_space)
	{
		return NULL;
	}
	
	
	return out_runner;
}

static size_t SumFreeSpaceIMP(BLOCK *block, size_t needed_space)
{
	ssize_t free_space_counter = 0;
	needed_space += HEADER_SIZE;
	while ((free_space_counter < needed_space) && (block->block_size < 0))
	{
		free_space_counter += abs(block->block_size) + HEADER_SIZE;
		block = NextBlockIMP(block);
	}
	free_space_counter -= HEADER_SIZE;
	
	if (free_space_counter < 0)
	{
		free_space_counter = 0;
	}
	
	return free_space_counter;
	/*(free_space_counter >= needed_space) ? free_space_counter : 0 ;*/

}

static BLOCK *NextBlockIMP(BLOCK *block)
{
	block = (BLOCK *)((char *)block + abs(block->block_size) + HEADER_SIZE);
	return block;
}

static size_t WordAlignedIMP (size_t block_size)
{
	if (((block_size) % WORD_LENGTH) != 0)
	{
		return ((block_size) / 
				WORD_LENGTH ) * WORD_LENGTH + WORD_LENGTH;
	}

	return block_size;
}
