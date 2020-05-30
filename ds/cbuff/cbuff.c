/********************************************
*	Author : Maoz Roytman				
*	Reviewer : 	Sharon Rottner
*	Cyrcular Buffer						
*	Date: 10.11.2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <string.h> /*memcpy*/
#include <assert.h> /*memcpy*/

#include "cbuff.h"

struct CB{
	size_t capacity;
	char *r_ptr;
	char *w_ptr;
	char begin[1];
};

static size_t min(size_t x, size_t y)
{
	if (x < y)
	{
		return x;
	}
	
	return y;
}

static size_t SpaceLeft(cbuff_t *cbuff, char *ch_ptr)
{
	return (cbuff->capacity + 1 - (ch_ptr - cbuff->begin));
}

cbuff_t *CBuffCreate(size_t capacity)
{	
	cbuff_t *new_buff = NULL;
	
	assert(capacity);
	
	new_buff = (cbuff_t *)malloc(sizeof(cbuff_t) + capacity);
	if (NULL == new_buff)
	{
		return 0;	
	}
	
	new_buff->capacity = capacity;
	new_buff->r_ptr = new_buff->begin;
	new_buff->w_ptr = new_buff->begin;
	
	return new_buff;
}

void CBuffDestroy(cbuff_t *cbuff)
{
	assert(cbuff);	

	free(cbuff);
	cbuff = NULL;
}

ssize_t CBuffRead(cbuff_t *cbuff, void *dest, size_t nbytes)
{
	ssize_t bytes_to_read = 0;
	ssize_t bytes_actually_read = 0;
	ssize_t first_read = 0;
	char *ch_dest = NULL;
	
	assert(cbuff);
	assert(dest);
	
	bytes_to_read = min((cbuff->capacity - CBuffSpaceLeft(cbuff)), nbytes);
	bytes_actually_read = bytes_to_read;
	first_read = min(bytes_to_read, SpaceLeft(cbuff, cbuff->r_ptr));
	ch_dest = (char *)dest;
	
	memcpy(ch_dest, cbuff->r_ptr, first_read);
	ch_dest += first_read;
	bytes_to_read -= first_read;
	cbuff->r_ptr += first_read;

	if (0 == SpaceLeft(cbuff, cbuff->r_ptr))
	{
		cbuff->r_ptr = cbuff->begin;
	}
	
	memcpy(ch_dest, cbuff->r_ptr, bytes_to_read);
	cbuff->r_ptr += bytes_to_read;
	
	return bytes_actually_read;
}

ssize_t CBuffWrite(cbuff_t *cbuff, const void *src, size_t nbytes)
{
	ssize_t bytes_to_write = 0;
	ssize_t bytes_actually_written = 0;
	ssize_t first_cpy = 0;
	char *ch_src = NULL;
	
	assert(cbuff);
	assert(src);
	
	bytes_to_write = min(CBuffSpaceLeft(cbuff), nbytes);
	bytes_actually_written = bytes_to_write;
	first_cpy = min(bytes_to_write, SpaceLeft(cbuff, cbuff->w_ptr));
	ch_src = (char*)src;
	
	memcpy(cbuff->w_ptr, ch_src, first_cpy);
	ch_src += first_cpy;
	bytes_to_write -= first_cpy;
	cbuff->w_ptr +=first_cpy;

	if (0 == SpaceLeft(cbuff, cbuff->w_ptr))
	{
		cbuff->w_ptr = cbuff->begin;
	}
	
	memcpy(cbuff->w_ptr, ch_src, bytes_to_write);
	cbuff->w_ptr += bytes_to_write;
	
	return bytes_actually_written;
}
  
size_t CBuffSpaceLeft(const cbuff_t *cbuff)
{
	assert(cbuff);

	if (cbuff->r_ptr > cbuff->w_ptr)
	{
		return ((size_t)(cbuff->r_ptr - cbuff->w_ptr) - 1);
	}
	
	if (cbuff->w_ptr > cbuff->r_ptr)
	{
		return (cbuff->capacity - (size_t)(cbuff->w_ptr - cbuff->r_ptr));
	}
	
	return cbuff->capacity;
}
 
int CBuffIsEmpty(const cbuff_t *cbuff)
{
	assert(cbuff);

	return (cbuff->r_ptr == cbuff->w_ptr);
}

size_t CBuffCapacity(const cbuff_t *cbuff)
{
	assert(cbuff);
	
	return (cbuff->capacity);
}
