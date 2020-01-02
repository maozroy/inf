 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : 	Yonatan V
*	pq					
*	Date: Mon Nov 18 20:16:13 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/

#include "pq.h"
#include "../heap/heap.h"

int MyIsBefore(const void *new_data, const void *src_data, void *param);

enum states
{
	SUCCESS = 0,
	FAIL = 1
};

struct p_queue
{
	heap_t *heap;
};

p_queue_t *PQCreate(void *priority_param, int(*compare_func)
					(const void *new_data, const void *src_data, void *param))
{
	p_queue_t *new_pq = (p_queue_t *)malloc(sizeof(p_queue_t));
	if (NULL == new_pq)
	{
		return NULL;
	}
	
	assert(compare_func);
	
	new_pq -> heap = HeapCreate(compare_func, priority_param);
	if (NULL == new_pq -> heap)
	{
		free(new_pq);
		return NULL;
	}

	return new_pq;
}

void PQDestroy(p_queue_t *p_queue)
{
	assert(p_queue);
	
	HeapDestroy(p_queue -> heap);
	free(p_queue);
}

int PQEnqueue(p_queue_t *p_queue, void *data)
{
	assert(p_queue);

	 return HeapPush(p_queue -> heap, data);
}

size_t PQSize(const p_queue_t *p_queue)
{
	assert(p_queue);

	return HeapSize(p_queue -> heap);
}

int PQIsEmpty(const p_queue_t *p_queue)
{
	assert(p_queue);

	return HeapIsEmpty(p_queue -> heap);
}

void *PQPeek(const p_queue_t *p_queue)
{
	assert(p_queue);

	return HeapPeek(p_queue -> heap);
}

void *PQDequeue(p_queue_t *p_queue)
{
	void *data = NULL;
	
	assert(p_queue);
	
	if (!PQIsEmpty(p_queue))
	{
		data = PQPeek(p_queue);
		HeapPop(p_queue -> heap);
	}
	return data;
}

void PQClear(p_queue_t *p_queue)
{
	assert(p_queue);

	while (!PQIsEmpty(p_queue))
	{
		HeapPop(p_queue -> heap);
	}
}

void *PQErase(void *data, p_queue_t *p_queue, 
 int(*is_match)(const void *new_data, const void *param))
{
	assert(p_queue);
	assert(is_match);
	
	return HeapRemove(p_queue -> heap, is_match, data); 
}



