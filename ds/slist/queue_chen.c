/************************************
*		Author: ChenR				*
*		Reviewer: Maoz				*
*		ds_queue					*
*		7/11/2019					*
*									*
************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include "queue.h"
#include "slist.h"

struct queue
{
    sl_node_t *rear;
    sl_node_t *front;
};

queue_t *QCreate()
{
	queue_t *new_queue = NULL;
	sl_node_t *dummy_node = NULL;
	int a1 = 1;
	
	new_queue = (queue_t *)malloc(sizeof(queue_t));
	if (NULL == new_queue)
	{
		return NULL;
	}
	
	dummy_node = SListCreateNode(&a1, NULL);
	
	new_queue -> rear = dummy_node;
	new_queue -> front = dummy_node;	
	
	return new_queue;
}

void QDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SListFreeAll(queue -> front);
	free(queue);	
}

int QEnqueue(queue_t *queue, void *data)
{
	sl_node_t *new_node = NULL;
	
	assert(NULL != queue);
	assert(NULL != data);
	
	if (NULL == queue || NULL == data)
	{
		return 1;
	}
	
	new_node = SListCreateNode(data, NULL);
	
	SListInsert(new_node, queue -> rear);
	queue -> rear = (queue -> rear) -> next;
	
	return 0;
}

void *QPeek(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (queue -> front) -> data;
}

void QDequeue(queue_t *queue)
{
	sl_node_t *temp_node = NULL;
	
	assert(NULL != queue);
	
	temp_node = (queue -> front) -> next;
	free(queue -> front);
	queue -> front = temp_node;
}

int QIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return ((queue -> front) == (queue -> rear));
}

size_t QSize(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (SListCount(queue -> front) - 1);
}

void QAppend(queue_t *dest, queue_t *src)
{
	sl_node_t *removed_node = (src -> front);
	
	assert(dest);
	assert(src);	
	
	(dest -> rear) -> next = (src -> front);
	removed_node = (src -> front);
	SListRemove(dest -> rear);
	free(removed_node);	
	(dest -> rear) = (src -> rear);
	free(src);	
}
