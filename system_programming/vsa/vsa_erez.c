/****************************************************************************
*                                                                           *
*                               OL79 - 01/12/19                             *
*                               Auther - Erez                               *	
*                               Variable Size Alloc	                        *
*                               Reviewer -Maoz                              *
*****************************************************************************/

#include <assert.h> /* assert */
#include <stdio.h>
#include <stdlib.h>

#include "vsa.h"

#define BYTE_SIZE 8
#define WORD_SIZE sizeof(size_t)
#define MY_MAGIC 64978794
#define MAX(x,y) ((x) > (y) ? (x) :(y))

typedef struct block_header
{
	ssize_t block_size;
	#ifdef NDEBUG
        size_t magic_number;
    #endif
} vsa_block_header_t;

static vsa_t *NewAllocFirstHeaderIMP
				(vsa_t *vsa,size_t block_size,size_t header_size);
static vsa_t *NewAllocEndHeaderIMP
				(vsa_t *first_header,size_t block_size,size_t header_size);
static vsa_t *JumpToNextHeader(vsa_t *head, size_t header_size);
static size_t AlignToWordSize(size_t data);
/**************************************************/
vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	vsa_t *new_vsa = memory_pool;
	size_t header_size = sizeof(vsa_t);
	vsa_t *end_dummy = NULL;
	
	assert(memory_pool);
	assert(pool_size);	
	
	header_size = AlignToWordSize(sizeof(vsa_t));
	
	new_vsa->block_size = -pool_size + (2 * header_size);
	end_dummy = (vsa_t*)((char*)memory_pool + pool_size - header_size);	
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
	size_t header_size = 0;

	assert(vsa);
	assert(block_size);
	
	header_size = AlignToWordSize(sizeof(vsa_t));
	block_size = AlignToWordSize(block_size);

	if (VSALargestChunk(vsa) < block_size)
	{
		return (NULL);
	}

	new_header_start = NewAllocFirstHeaderIMP(vsa, block_size, header_size);
	new_header_end = 
				NewAllocEndHeaderIMP(new_header_start, block_size, header_size);

	new_header_start->block_size = 
			((char*)(new_header_end) - (char*)new_header_start - header_size);	

	new_header_start = (vsa_t*)((char*)new_header_start + header_size);

	return (new_header_start);
}
/*********/
void VSAFree(void *allocated_address)
{  	
	vsa_t *block = (vsa_t*)((char*)allocated_address - sizeof(vsa_t));

	assert(allocated_address);
	if (allocated_address == NULL)
	{
	return;
	}
	#ifdef NDEBUG
	if(block->magic_number != MY_MAGIC)
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
	ssize_t header_size = AlignToWordSize(sizeof(vsa_t));
	
	assert(vsa);

	curr_chunk = -header_size;
	
	while(0 != block_runner->block_size)
	{
		if(0 > block_runner->block_size)
		{
			curr_chunk -= block_runner->block_size - header_size;
			max_chunk = MAX(max_chunk, curr_chunk);
		}
		else 
		{
			curr_chunk = -header_size;
		}

		block_runner = JumpToNextHeader(block_runner, header_size);
	}

	return (max_chunk);
}
/*********/
static vsa_t *NewAllocFirstHeaderIMP
				(vsa_t *vsa,size_t block_size,size_t header_size)
{
	vsa_t *first_header = vsa;
	vsa_t *last_header = vsa;
	size_t my_header_size = 0;	
	ssize_t block_size_runner = -my_header_size;

	assert(vsa);
	assert(block_size);
	assert(header_size);	
	
	my_header_size = AlignToWordSize(sizeof(vsa_t));	
	while (block_size_runner < (ssize_t)block_size)
	{
		if (last_header->block_size < 0)
		{
			block_size_runner += (header_size - last_header->block_size);
			last_header = JumpToNextHeader(last_header, header_size);
		}
		else
		{
			block_size_runner = -header_size;
			last_header = JumpToNextHeader(last_header, header_size);
			first_header = last_header;
		}
	}
	
	return (first_header);		
}
/*********/
static vsa_t *NewAllocEndHeaderIMP
				(vsa_t *first_header,size_t block_size,size_t header_size)
{
	vsa_t *last_header = first_header;	
	size_t my_header_size = 0;		
	ssize_t block_size_runner = -my_header_size;
	
	my_header_size = AlignToWordSize(sizeof(vsa_t));		
		
	while(block_size_runner <= (ssize_t)block_size)
	{
		block_size_runner += (header_size - last_header->block_size);
		last_header = JumpToNextHeader(last_header, header_size);		
	}

	if ((block_size_runner - block_size) > header_size + WORD_SIZE)
	{
		last_header = (vsa_t*)((char*)first_header + block_size + header_size);	
		last_header->block_size = 
						-block_size_runner + block_size + 2 * header_size;
		
		#ifdef NDEBUG
		last_header->magic_number = MY_MAGIC;
	    #endif
	}

	return (last_header);
}
/*********/
static vsa_t *JumpToNextHeader(vsa_t *head, size_t header_size)
{
	return	((vsa_t*)((char*)head + abs(head->block_size) + header_size));
}
/*********/
static size_t AlignToWordSize(size_t data)
{
	if (data % WORD_SIZE)
	{
	
		return (data + WORD_SIZE - (data % WORD_SIZE));	
	}
	
	return (data);
}
/*********/
