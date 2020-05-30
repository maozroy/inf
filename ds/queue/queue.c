/********************************************
*	Author : Maoz Roytman				
*	Reviewer : 	Tamir Dayan
*	Queue						
*	Date: 7.11.2019		                
*																				
********************************************/


#include <stdlib.h> /*mallocing*/
#include <assert.h> /*asserting  !*/

#include "queue.h" /*queue header*/
#include "../slist/slist.h" /*slist header*/

struct queue
{
    sl_node_t *rear;
    sl_node_t *front;
};

queue_t *QCreate()
{
	sl_node_t *dummy = NULL;
	queue_t *new_queue = (queue_t*)malloc(sizeof(queue_t));
	if (NULL == new_queue)
	{
		return NULL;
	}
	
	dummy = SListCreateNode("66666",NULL);
	if (NULL == dummy)
	{
		free(new_queue);
		return NULL;
	}
	
	new_queue->rear = dummy;
	new_queue->front = dummy;

	return new_queue;
}

void QDestroy(queue_t *queue)
{
	assert(queue);

	SListFreeAll(queue->front);
	free(queue);
}

int QEnqueue(queue_t *queue, void *data)
{
	sl_node_t *new_node = NULL;
	
	assert(queue);
	assert(data);
	
	new_node = SListCreateNode(data, NULL);
	if (NULL == new_node)
	{
		return 1;
	}
	
	SListInsert(new_node,queue->rear);
	queue->rear = queue->rear->next;
	
	return 0;
}

void QDequeue(queue_t *queue)
{
	sl_node_t *node_to_remove;
	
	assert(queue);
	
	node_to_remove = queue->front;
	queue->front=queue->front->next;	
	free(node_to_remove);
	node_to_remove = NULL;
}

void *QPeek(const queue_t *queue)
{
	assert(queue);

	return (queue->front->data);
}

int QIsEmpty(const queue_t *queue)
{
	assert(queue);
	
	return (QSize(queue)==0);
}

size_t QSize(const queue_t *queue)
{
	assert(queue);

	return (SListCount(queue->front) - 1);
}

void QAppend(queue_t *dest, queue_t *src)
{
	sl_node_t *holder = NULL;
	
	assert(dest);
	assert(src);
	
	dest->rear->next = src->front;
	holder = dest->rear->next;
	SListRemove(dest->rear);
	dest->rear = src->rear;
	free(src);
	free(holder);
	src = NULL;
}
