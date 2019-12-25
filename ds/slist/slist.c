/********************************************
*	Author : Maoz Roytman				
*	Reviewer : 	Gal Salemon
*	Singly List								
*	Date: 6.11.2019		                
*																				
********************************************/
#include <assert.h> /*asserting*/
#include <stdlib.h> /*malloc & free*/
#include "slist.h" /*header*/

static void SwapPointers(void **ptr1, void **ptr2);

sl_node_t *SListCreateNode(void *data, sl_node_t *next)
{
	sl_node_t *new_node = malloc(sizeof(sl_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	assert(data);
	
	new_node->data = data;
	new_node->next = next;
	
	return new_node;
}

sl_node_t *SListInsert(sl_node_t *new_node, sl_node_t *pos)
{
	assert(new_node);
	assert(pos);
	
	SwapPointers((void*)&new_node->data, (void*)&pos->data);
	new_node->next = pos->next;
	pos->next = new_node;
	
	return pos;
}

sl_node_t *SListInsertAfter(sl_node_t *new_node, sl_node_t *pos)
{
	assert(new_node);
	assert(pos);
	
	if (pos->next != NULL)
	{
		new_node->next = pos->next;
	}
	pos->next = new_node;
	
	return new_node;
}

sl_node_t *SListRemove(sl_node_t *node)
{
	assert(node);

	SwapPointers((void*)&node->data, (void*)&node->next->data);
	SListRemoveAfter(node);
	
	return node;
}

size_t SListCount(const sl_node_t *head)
{
	size_t counter = 1;
	sl_node_t *runner = (void*)head;
	
	assert(head);
	
	while (runner->next != NULL)
	{
		counter++;
		runner = runner->next;
	}
	
	return counter;
}

sl_node_t *SListRemoveAfter(sl_node_t *node)
{	
	sl_node_t *hold = node->next;
	
	assert(node);
	
	node->next = node->next->next;
	hold->next = NULL;
	
	return node;
}

void SListFreeAll(sl_node_t *node)
{
	assert(node);
	
	if (node->next != NULL)
	{
		SListFreeAll(node->next);
	}

	free(node);
}

sl_node_t *SListFlip(sl_node_t *head)
{
	sl_node_t *next_next = NULL;
	sl_node_t *next = NULL;
	sl_node_t *node = head;

	assert(head);

	if (head->next == NULL)
	{	
		return head;
	}

	if (head->next->next == NULL)
	{
		next = head->next;
		head->next = NULL;
		
		return next;
	}
	
	next = head->next;
	next_next = next->next;
	
	while (next_next->next != NULL )
	{
		next->next = node;
		node = next;
		next = next_next;
		next_next = next_next->next;
	}
	
	next->next = node;
	next_next->next = next;
	head->next = NULL;
	
	return next_next;
}

int SListHasLoop (sl_node_t *head)
{
	sl_node_t *slow = head;
	sl_node_t *fast = head;

	assert(head);
	
	if (head->next == NULL)
	{
		return (0);
	}
	
	while (fast->next != NULL)
	{
		if (fast->next == slow)
		{
			return 1;
		}
		
		slow = slow->next;
		fast = fast->next;
		
		if (fast->next == NULL)
		{
			return 0;
		}
		fast = fast->next;
	}

	return(0);
}

const sl_node_t *SListFind(const sl_node_t *node, void *param, action_ptr ptr)
{	
	assert(node);
	assert(ptr);
	assert(param);

	while (node->next != NULL)
	{
		if (ptr(node, param) == 1)
		{
			return node;
		}
		
		node = node->next;
	}
	
	if (ptr(node, param) == 1)
	{
		return node;
	}
	
	return NULL;
}

int SListForEach(sl_node_t *node, void *param, for_each_ptr ptr)
{
	assert(node);
	assert(ptr);
	assert(param);
	
	while (node->next != NULL)
	{
		if (ptr(node, param) == 1)
		{
			return 1;
		}
		
		node=node->next;
	}
	
	if (ptr(node, param) == 1)
	{
		return 1;
	}
	
	return 0;
}

sl_node_t *SListFindIntersection (sl_node_t *node1, sl_node_t *node2)
{
	sl_node_t *end1 = node1;
	sl_node_t *end2 = node2;
	sl_node_t *intersection = NULL;
	size_t num_of_nodes1 = 0;
	size_t num_of_nodes2 = 0;
	size_t disc = 0;
	size_t disc_keeper = 0;
	
	assert(node1);
	assert(node2);
	
	while (end1->next != NULL)
	{
		end1 = end1->next;
		++num_of_nodes1;
	}
	
	while (end2->next != NULL)
	{
		end2 = end2->next;
		++num_of_nodes2;
	}
	
	if (end1 != end2)
	{
		return NULL;
	}
	
	if (num_of_nodes1 > num_of_nodes2)
	{
		disc = num_of_nodes1 - num_of_nodes2;
		disc_keeper = disc;
		end1 = node1;
		
		while (disc > 0)
		{
			 end1 = end1->next;
			 --disc;
		}
		
		if (end1 == node2)
		{
			return NULL;
		}
		
		for (disc_keeper = disc ; disc_keeper > 0 ; --disc_keeper)
		{
			node1 = node1->next;
		}
		
		while (node1 != node2)
		{
			node1 = node1->next;
			node2 = node2->next;
			intersection = node1;
		}
	}
	
	if (num_of_nodes2 >= num_of_nodes1)
	{
		disc = num_of_nodes2 - num_of_nodes1;
		disc_keeper = disc;
		end2 = node2;
		
		while (disc > 0)
		{
			 end2 = end2->next;
			 --disc;
		}
		
		if (end2 == node1)
		{
			return NULL;
		}
		
		for (disc_keeper = disc ; disc_keeper > 0 ; --disc_keeper)
		{
			node2 = node2->next;
		}
		
		while (node1 != node2)
		{
			node1 = node1->next;
			node2 = node2->next;
			intersection = node2;
		}
	}
	
	return intersection;
}

static void SwapPointers(void **ptr1, void **ptr2)
{
	void *hold = *ptr1;
	
	*ptr1 = *ptr2;
	*ptr2 = hold;
}
