/************************************
*		Author: ChenR				  *
*		Reviewer: Gal				  *
*		PQ							  *
*		18/11/2019					  *
*									  *
************************************/
#include <stdlib.h> 	/* malloc */
#include <assert.h> 	/* assert */

#include "pq.h"			/* priority queue */
#include "dllist.h" 	/* doubly link list */
#include "sortedlist.h" /* sorted list */

/* wrapper function to use in SrtInsert */
int MyIsBefore(const void *new_data, const void *src_data, void *wrap);

typedef struct sort_params_wrapper
{
	int(*compare_func)(const void *new_data, const void *src_data, void *param);
	void *priority_param;
}sort_params_t;

struct p_queue
{
	srt_list_t *srt_list;
	sort_params_t compareWrapper;
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
	
	(new_pq -> compareWrapper).priority_param = priority_param;
	(new_pq -> compareWrapper).compare_func = compare_func;
	new_pq -> srt_list = 
	SrtListCreate(&(new_pq -> compareWrapper), MyIsBefore);
	if (NULL == new_pq -> srt_list)
	{
		free(new_pq);
		new_pq = NULL;
		
		return NULL;
	}
	
	return new_pq;
}

void PQDestroy(p_queue_t *p_queue)
{
	assert(p_queue);
	
	SrtListDestroy(p_queue -> srt_list);
	free(p_queue);
	p_queue = NULL;
}

int PQEnqueue(p_queue_t *p_queue, void *data)
{
	assert(p_queue);
	
	return (SrtListIsSameIterator(SrtListInsert(data, p_queue -> srt_list), 
								  SrtListEnd(p_queue -> srt_list)));
}

void *PQDequeue(p_queue_t *p_queue)
{
	assert(p_queue);
	
	return (SrtListPopFront(p_queue -> srt_list));
}

void *PQPeek(const p_queue_t *p_queue)
{
	assert(p_queue);
	
	return (SrtListGetData(SrtListBegin(p_queue -> srt_list)));
}

size_t PQSize(const p_queue_t *p_queue)
{
	return (SrtListSize(p_queue -> srt_list));
}

int PQIsEmpty(const p_queue_t *p_queue)
{
	assert(p_queue);
	
	return (SrtListIsEmpty(p_queue -> srt_list));
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
	srt_iter_t iter_to_Find = {0};
	void *res = NULL;
	
	assert(p_queue);
	
	iter_to_Find = SrtListFindIf(SrtListBegin(p_queue -> srt_list),
				  SrtListEnd(p_queue -> srt_list), data, is_match);
	res = SrtListGetData(iter_to_Find);			  
	
	if (SrtListIsSameIterator(iter_to_Find, SrtListEnd(p_queue -> srt_list)))
	{
		return NULL;
	}
	else
	{
		SrtListRemove(iter_to_Find);
		
		return res;
	}
}

/* this function is a wrapper that receives three void params and returns 1 or
0 as opposed to compare func that returns -1, 0 or 1*/
int MyIsBefore(const void *new_data, const void *src_data, void *param)
{
	sort_params_t *wrap = (sort_params_t *)param;
	
	if (1 == (wrap -> compare_func(new_data, src_data, 
								   wrap -> priority_param)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
