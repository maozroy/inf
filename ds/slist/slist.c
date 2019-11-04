#include <assert.h>
#include <stdlib.h>
#include "slist.h"

node_t *SListCreateNode(void *data, node_t *next)
{
	node_t *new_node = malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	new_node->data = data;
	new_node->next = next;
	
	return new_node;
}

node_t *SListInsert(node_t *new_node, node_t *pos)
{
	void *adrs_keeper = NULL;
	
	new_node->next = pos->next;
	pos->next = new_node;
	
	adrs_keeper = new_node->data;
	new_node->data = pos->data;
	pos->data=adrs_keeper;
	
	return new_node;

}

node_t *SListInsertAfter(node_t *new_node, node_t *pos)
{
	new_node->next = pos->next;
	pos->next = new_node;

	
	return new_node;
	
}

node_t *SListRemove(node_t *node)
{
	node_t *holder = node->next;
	
	if (NULL == node->next)
	{
		free(node);
	}
	else
	{
		node->data = (node->next)->data;
		node->next = (node->next)->next;
	
		free(holder);
	}
	return node;
}

size_t SListCount(const node_t *head)
{
	size_t counter = 1;
	node_t *runner = (void*)head;
	while(runner->next != NULL)
	{
		counter++;
		runner = runner->next;
		
	}
	
	return counter;
}

node_t *SListRemoveAfter(node_t *node)
{
	free(node->next);
	node->next = NULL;
	
	return node;
}

void SListFreeNode(node_t *node)
{
	while(node->next != NULL)
	{
		SListRemoveAfter(node);
	}

}



