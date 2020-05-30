/****************************************************************************
*                                                                           *
*                               OL79 - 01/12/19                             *
*                               Auther - Erez                               *	
*                               Variable Size Alloc	                        *
*                               Reviewer -Maoz                              *
*****************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h>

#include "vsa.h"

#define BYTE_SIZE 8
#define WORD_SIZE sizeof(size_t)
#define HEADER_SIZE AlignToWordSizeIMP(sizeof(vsa_t))
#define MY_MAGIC 64978794
#define MAX(x,y) ((x) > (y) ? (x) :(y))

typedef struct block_header
{
	ssize_t block_size;
	#ifdef NDEBUG
    size_t magic_number;
    #endif
} vsa_block_header_t;

/* Function returns the 1st header that has enough free space >= block_size*/ 
static vsa_t *NewAllocFirstHeaderIMP(vsa_t *vsa,ssize_t block_size);
/* Function returns end header for allocated space */
static vsa_t *NewAllocEndHeaderIMP(vsa_t *first_header,ssize_t block_size);
/* Function returns pointer to the next header */
static vsa_t *JumpToNextHeaderIMP(vsa_t *head);
/* Function returns size alindeg to WORD_SIZE */
static size_t AlignToWordSizeIMP(size_t data);
/**************************************************/
vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	vsa_t *new_vsa = memory_pool;
	vsa_t *end_dummy = NULL;
	
	assert(memory_pool);
	assert(pool_size);	
	
/*	pool_size	= AlignToWordSizeIMP(pool_size); */
	new_vsa->block_size = -pool_size + (2 * HEADER_SIZE);
	end_dummy = (vsa_t *)((char *)memory_pool + pool_size - HEADER_SIZE);	
	end_dummy->block_size = 0;

	#ifdef NDEBUG
	new_vsa->magic_number = MY_MAGIC;
	end_dummy->magic_number = MY_MAGIC;	
    #endif
    	
	return (new_vsa);
}
/*********/
void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	vsa_t *new_header_start = NULL;
	vsa_t *new_header_end = NULL;	

	assert(vsa);
	assert(block_size > 0);
	
	block_size = AlignToWordSizeIMP(block_size);

	new_header_start = NewAllocFirstHeaderIMP(vsa, block_size);
	if(NULL == new_header_start)
	{
		return (new_header_start);
	}
	
	new_header_end = NewAllocEndHeaderIMP(new_header_start, block_size);

	new_header_start->block_size = 
			((char *)(new_header_end) - (char *)new_header_start - HEADER_SIZE);	

	new_header_start = (vsa_t*)((char *)new_header_start + HEADER_SIZE);

	return (new_header_start);
}
/*********/
void VSAFree(void *allocated_address)
{  	
	vsa_t *block = NULL;
		
	assert(allocated_address);

	block = (vsa_t *)((char *)allocated_address - HEADER_SIZE);		

	#ifdef NDEBUG
	if (block->magic_number != MY_MAGIC)
	{
		return;	
	}
    #endif
    
	block->block_size = -abs(block->block_size);
}
/*********/
size_t VSALargestChunk(vsa_t *vsa)
{
	ssize_t max_chunk = 0; 
	ssize_t curr_chunk = 0; 	
	vsa_t *block_runner = vsa;
	
	assert(vsa);

	curr_chunk = -HEADER_SIZE;
	
	while (0 != block_runner->block_size)
	{
		if (0 > block_runner->block_size)
		{
			curr_chunk -= (block_runner->block_size - HEADER_SIZE);
			max_chunk = MAX(max_chunk, curr_chunk);
		}
		else 
		{
			curr_chunk = -HEADER_SIZE;
		}

		block_runner = JumpToNextHeaderIMP(block_runner);
	}

	return (max_chunk);
}
/*********/
static vsa_t *NewAllocFirstHeaderIMP(vsa_t *vsa,ssize_t block_size)
{
	vsa_t *first_header = vsa;
	vsa_t *last_header = vsa;
	ssize_t block_size_runner = -HEADER_SIZE;

	assert(vsa);
	assert(block_size);
	
	while (block_size_runner < (ssize_t)block_size && 0 != last_header->block_size)
	{
		if (last_header->block_size < 0)
		{
			block_size_runner += (HEADER_SIZE - last_header->block_size);
			last_header = JumpToNextHeaderIMP(last_header);
		}
		else
		{
			block_size_runner = -HEADER_SIZE;
			last_header = JumpToNextHeaderIMP(last_header);
			first_header = last_header;
		}
	}
	
	if(0 == last_header->block_size && block_size_runner < (ssize_t)block_size)
	{
		return (NULL);
	}
	
	return (first_header);		
}
/*********/
static vsa_t *NewAllocEndHeaderIMP(vsa_t *first_header,ssize_t block_size)
{
	vsa_t *last_header = first_header;	
	ssize_t block_size_runner = -0;

		
	while (block_size_runner <= (ssize_t)block_size)
	{
		block_size_runner += (HEADER_SIZE - last_header->block_size);
		last_header = JumpToNextHeaderIMP(last_header);		
	}

	if ((block_size_runner - block_size) > ((ssize_t)HEADER_SIZE + 2 * (ssize_t)WORD_SIZE))
	{
		last_header = (vsa_t *)((char *)first_header + block_size + HEADER_SIZE);	
		last_header->block_size = 
						-block_size_runner + block_size + 2 * HEADER_SIZE;
		
		#ifdef NDEBUG
		last_header->magic_number = MY_MAGIC;
	    #endif
	}

	return (last_header);
}
/*********/
static vsa_t *JumpToNextHeaderIMP(vsa_t *head)
{
	if (head->block_size)
	{
	return	((vsa_t *)((char *)head + abs(head->block_size) + HEADER_SIZE));
	}
	
	return (head);
}
/*********/
static size_t AlignToWordSizeIMP(size_t data)
{
	if (data % WORD_SIZE)
	{
	
		return (data + WORD_SIZE - (data % WORD_SIZE));	
	}
	
	return (data);
}
/*********/
