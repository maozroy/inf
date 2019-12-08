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
#include "dllist.h"
#include "sortedlist.h"

int MyIsBefore(const void *new_data, const void *src_data, void *param);

typedef struct sort_params_wrapper
{
	int(*compare_func)(const void *new_data, const void *src_data, void *param);
	void *priority_param;
}compare_wrapper_t;

struct p_queue
{
	srt_list_t *srt_list;
	compare_wrapper_t compare_wrapper;
};

p_queue_t *PQCreate(void *priority_param, int(*compare_func)
					(const void *new_data, const void *src_data, void *param))
{
	p_queue_t *new_pq = (p_queue_t *)malloc(sizeof(p_queue_t));
	if(NULL == new_pq)
	{
		return NULL;
	}
	
	assert(compare_func);
	
	new_pq->compare_wrapper.priority_param = priority_param;
	new_pq->compare_wrapper.compare_func = compare_func;
	new_pq->srt_list = SrtListCreate(&(new_pq->compare_wrapper), MyIsBefore);
	if(NULL == new_pq->srt_list)
	{
		free(new_pq);
		new_pq = NULL;
		
		return NULL;
	}

	return new_pq;
}

int MyIsBefore(const void *new_data, const void *src_data, void *param)
{
	compare_wrapper_t *wrap = param;
	int result = 0;
	
	result = wrap->compare_func(new_data, src_data, wrap->priority_param);
	
	if (-1 == result)
	{
		result = 0;
	}
	
	return result;
}

void PQDestroy(p_queue_t *p_queue)
{
	assert(p_queue);
	
	SrtListDestroy(p_queue->srt_list);
	free(p_queue);
	p_queue = NULL;
}

int PQEnqueue(p_queue_t *p_queue, void *data)
{
	assert(p_queue);

	 return SrtListIsSameIterator(SrtListEnd(p_queue->srt_list), 
			SrtListInsert(data, p_queue->srt_list));
}

size_t PQSize(const p_queue_t *p_queue)
{
	assert(p_queue);

	return SrtListSize(p_queue->srt_list);
}

int PQIsEmpty(const p_queue_t *p_queue)
{
	assert(p_queue);

	return SrtListIsEmpty(p_queue->srt_list);
}

void *PQPeek(const p_queue_t *p_queue)
{
	assert(p_queue);

	return SrtListGetData(SrtListBegin(p_queue->srt_list));
}

void *PQDequeue(p_queue_t *p_queue)
{
	assert(p_queue);

	return SrtListPopFront(p_queue->srt_list);
}

void PQClear(p_queue_t *p_queue)
{
	assert(p_queue);

	while (!PQIsEmpty(p_queue))
	{
		PQDequeue(p_queue);
	}
}

void *PQErase(void *data, p_queue_t *p_queue, 
 int(*is_match)(const void *new_data, const void *param))
{
	srt_iter_t iter = {0};
	void *ptr = NULL;
	
	assert(p_queue);
	assert(is_match);
	
	iter = SrtListFindIf(SrtListBegin(p_queue->srt_list), 
						SrtListEnd(p_queue->srt_list), data, is_match);
	if (!SrtListIsSameIterator(SrtListEnd(p_queue->srt_list), iter))
	{
		ptr = SrtListGetData(iter);
		SrtListRemove(iter);
	}

	return ptr;
}
