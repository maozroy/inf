#include <stdlib.h> /*mallocing*/
#include <assert.h> /*asserting*/
#include "vector.h"

void CopyVoidToVoid(void *src, void *dst, size_t n); 

struct d_vector_t
{
	size_t capacity;
	size_t element_size;
	size_t size;
	void *head;
};


d_vector_t *VectorCreate(size_t capacity, size_t elements_size)
{
	d_vector_t *vector = malloc(sizeof(d_vector_t));
	
	if (NULL == vector)
	{
		return 0;
	}
	
	vector->head = malloc(capacity * elements_size);
	if (NULL == vector->head)
	{
		return 0;
	}
	
	vector->element_size = elements_size;
	vector->size = 0;
	vector->capacity = capacity;
	
	return vector;
}

int VectorReserve(d_vector_t *vector, size_t new_capacity)
{
	void *ptr_holder = vector->head;
	
	assert(vector);
	
	if (new_capacity <= (vector->size))
	{
		new_capacity = vector->size;
	}
	
	vector->head = realloc(vector->head, (new_capacity*vector->element_size) );
	if (NULL == vector->head)
	{
		vector->head = ptr_holder;
		
		return (1);
	}
	
	vector->capacity = new_capacity;
	
	return (0);
}

int VectorPushBack(d_vector_t *vector, const void *n)
{
	assert(vector);
	
	if (vector->size == vector->capacity)
	{
		VectorReserve(vector, (vector->capacity) * 2);
	}
	CopyVoidToVoid((void*)n, (void*)((char*)vector->head+(vector->size*vector->element_size)), vector->element_size);
	vector->size += 1;
	
	return 0;
}

void CopyVoidToVoid(void *src, void *dst, size_t n)
{
	size_t i = 0;
	
	assert(src);
	assert(dst);
	
	for (i = 0 ; n > i ; i++)
	{
		*(char*) dst = *(char*) src;
		dst = ((char*)dst) + 1;
		src = ((char*)src) + 1;
	}
}

void VectorDestroy(d_vector_t *vector)
{
	assert(vector);

	free(vector->head);
	/*vector->head = NULL;*/
	
	free(vector);
}

void VectorPopBack(d_vector_t *vector)
{	
	assert(vector);

	vector->size -= 1;
	
	if (vector->size < (vector->capacity / 2))
	{
		VectorReserve(vector, (vector->capacity) / 2);
	}
	
}

void *VectorGetItemAddress(const d_vector_t *vector, size_t index)
{	
	assert(vector);

	return (char*)vector->head + (index * vector->element_size);
}

size_t VectorCapacity(const d_vector_t *vector)
{
	assert(vector);
	
	return vector->capacity;
}

size_t VectorSize(const d_vector_t *vector)
{
	assert(vector);

	return vector->size;
}
