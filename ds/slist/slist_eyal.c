/****************************************************************************
*		  	                OL79 - 6/11/19		                            *
*          Worksheet 2 - Data Structures  :  singly linked list             * 
*                   Author: Eyal R      Reviewer: Maoz                      *
*****************************************************************************/

#include <assert.h> /* assert */ 
#include <stdlib.h> /* malloc */ 
#include "slist.h"

/* swap two pionters */
void swap(void **x, void **y);

sl_node_t *SListCreateNode(void *data, sl_node_t *next)
{
	sl_node_t *node = (sl_node_t*)malloc(sizeof(sl_node_t));
	if(NULL == node)
	{
		return NULL;
	}

	node -> data = data;
	node -> next = next;

	return node;
}

void SListFreeAll(sl_node_t *node)
{
	assert(NULL != node);

	if(NULL != node -> next)
	{
		SListFreeAll(node -> next);
	}
	free(node);
}

void swap(void **x, void **y)
{
	void *temp = *x;
	*x = *y;
	*y = temp;
}

sl_node_t *SListInsertAfter(sl_node_t *new_node, sl_node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);

	if(NULL != pos -> next)
	{
		new_node -> next = pos -> next;
	}
	pos -> next = new_node;

	return new_node;
}

sl_node_t *SListInsert(sl_node_t *new_node, sl_node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);

	SListInsertAfter(new_node, pos);
	swap(&new_node -> data, &pos -> data);

	return pos;
}

size_t SListCount(const sl_node_t *head)
{
	size_t count = 1;
	assert(NULL != head);

	while(NULL != head -> next)
	{
		++count;
		head = head -> next;
	}

	return count;
}

sl_node_t *SListRemove(sl_node_t *node)
{
	sl_node_t *node_to_remove = node -> next;
	assert(NULL != node);

	node -> data = node_to_remove -> data;
	node -> next = node_to_remove -> next;
	(node_to_remove) -> next = NULL;
	
	return node;	
}

sl_node_t *SListRemoveAfter(sl_node_t *node)
{
	sl_node_t *node_to_remove = node -> next;
	assert(NULL != node);

	if(NULL == node -> next)
	{
		return(node);
	}

	node -> next = (node -> next) -> next;
	node_to_remove -> next = NULL;

	return node;
}

sl_node_t *SListFlip(sl_node_t *head)
{
	sl_node_t *current_node = head;
	sl_node_t *next_node = head -> next;
	sl_node_t *next_node_next = NULL; 
	assert(NULL != head);

	if(NULL == head -> next)
	{
		return head;
	}

	current_node -> next = NULL;

	while(NULL != next_node)
	{
		next_node_next = next_node -> next;
		next_node -> next = current_node;
		current_node = next_node;
		next_node = next_node_next;
	}

	return current_node;
}

int SListForEach(sl_node_t *node, void *param, for_each_ptr ptr)
{
	assert(NULL != node);
	assert(NULL != param);

	while(NULL != node)
	{
		if(0 == ptr(node, param))
		{	
			node = node -> next;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

const sl_node_t *SListFind(const sl_node_t *node, void *param, find_ptr ptr)
{
	assert(NULL != node);
	assert(NULL != param);

	while(NULL != node)
	{
		if(1 == ptr(node, param))
		{
			return node;
		}
		node = node -> next;
	}

	return NULL;
}

int SListHasLoop (sl_node_t *head)
{
	sl_node_t *head_run = head;
	sl_node_t *next_head = head_run -> next;
	int count = 0;
	int update_count = 1;
	assert(NULL != head);

	if(NULL == head_run -> next)
	{
		return 0;
	}

	while(update_count > count)
	{
		count = update_count;
		head_run = next_head;

		if(NULL != head_run -> next)
		{
			next_head = head_run -> next;
			head_run -> next = NULL;
			update_count = SListCount(head);
			head_run -> next = next_head;
		}
		else
		{
			return 0;
		}
	}

	return 1;
}

sl_node_t *SListFindIntersection (sl_node_t *node1, sl_node_t *node2)
{
	sl_node_t *node2_run = node2;
	assert(NULL != node1);
	assert(NULL != node2);

	while(NULL != node1)
	{
		node2_run = node2;
		while((NULL != node2_run) && (node1 != node2_run))
		{
			if(node1 -> next == node2_run)
			{
				if(node2_run == node2)
				{
					return NULL;
				}
				return node2_run;
			}
			node2_run = node2_run -> next;
		}
		node1 = node1 -> next;
	}

	return NULL;
}


