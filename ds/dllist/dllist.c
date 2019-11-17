 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : Erez Barr
*	dllist	
*	Date: Nov 12 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*assert*/
#include <stdio.h> /*printing*/

#include "dllist.h"

struct dl_list
{
	dll_node_t *begin;
	dll_node_t *end;
};

struct node 
{
	void *data;
	struct node *prev;
	struct node *next;
};

static dll_node_t *DLListCreateNode(
		void *data, dll_node_t *next, dll_node_t *prev);
static void DestroyNodes(dll_node_t *node);

dl_list_t *DLListCreate(void)
{
	dll_node_t *dummy_begin = NULL;
	dll_node_t *dummy_end = NULL;
	dl_list_t *new_list = (dl_list_t *)malloc(sizeof(dl_list_t));
	if (NULL == new_list)
	{
		return NULL;
	}
	
	dummy_begin = DLListCreateNode((void *)0xDEADBEEF,NULL,NULL);
	if (NULL == dummy_begin)
	{
		free(new_list);
		new_list = NULL;
		
		return NULL;
	}


	dummy_end = DLListCreateNode((void *)0xDEADBEEF, NULL ,NULL);
	if (NULL == dummy_end)
	{
		free(new_list);
		free(dummy_begin);
		new_list = NULL;
		dummy_begin = NULL;
		
		return NULL;
	}
	
	new_list->begin = dummy_begin;
	new_list->end = dummy_end;
	(new_list->begin)->next = dummy_end;
	(new_list->end)->prev = dummy_begin;
	
	return new_list;
}

void DLListDestroy(dl_list_t *list)
{
	assert(list);
	
	DestroyNodes(list->begin);
	free(list);
}

dll_iter_t DLListPushFront(dl_list_t *list, void *data)
{
	dll_node_t *new_node = NULL;
	
	assert(list);
	
	new_node = DLListCreateNode(data, NULL, NULL);
	if (NULL == new_node)
	{
		return DLListEnd(list);
	}
	
	new_node->next = list->begin->next;
	new_node->prev = list->begin;
	(list->begin->next)->prev = new_node;
	(list->begin)->next = new_node;

	return new_node;
}

dll_iter_t DLListPushBack(dl_list_t *list, void *data)
{
	dll_node_t *new_node = NULL;
	
	assert(list);
	
	new_node = DLListCreateNode(data, NULL, NULL);
	if (NULL == new_node)
	{
		return DLListEnd(list);
	}
	
	new_node->next = list->end;
	new_node->prev = list->end->prev;
	(list->end->prev)->next = new_node;
	(list->end)->prev = new_node;

	return new_node;
}

dll_iter_t DLListInsert(void *data, dll_iter_t iterator, dl_list_t *list)
{
	dll_node_t *new_node = NULL;

	assert(iterator);
	assert(list);

	new_node = DLListCreateNode(data, NULL, NULL);
	if (NULL == new_node)
	{
		return DLListEnd(list);
	}
	
	new_node->next = iterator;
	new_node->prev = iterator->prev;
	iterator->prev->next = new_node;
	iterator->prev = new_node;
	
	return new_node;	
}

void *DLListPopFront(dl_list_t *list)
{
	void *ptr = NULL;
	
	assert(list);
	
	ptr = DLListGetData(DLListBegin(list));
	DLListRemove(DLListBegin(list));
	
	return ptr;
}

void *DLListPopBack(dl_list_t *list)
{
	void *ptr = NULL;
	
	assert(list);
	
	ptr = DLListGetData(DLListPrev(DLListEnd(list)));
	DLListRemove(DLListPrev(DLListEnd(list)));
	
	return ptr;
}

dll_iter_t DLListRemove(dll_iter_t iterator)
{
	dll_node_t *prev_node = NULL;
	
	assert(iterator);
	
	prev_node = iterator->prev;
	iterator->prev->next = iterator->next;
	iterator->next->prev = prev_node;
	free(iterator);
	
	return prev_node->next;
}

dll_iter_t DLListSplice(dll_iter_t s_begin, dll_iter_t s_end, dll_iter_t dest)
{
	assert(s_begin);
	assert(s_end);
	assert(dest);
	
	(s_end->prev)->next = dest->next;
	(s_end)->prev = s_begin->prev;

	(dest->next)->prev = s_end->prev;
	(s_begin->prev)->next = s_end;
	(s_begin)->prev = dest;
	(dest)->next = s_begin;
	
	return dest;
}

size_t DLListSize(const dl_list_t *list)
{
	dll_node_t *node = NULL;
	size_t counter = 0;
	
	assert(list);
	
	node = DLListBegin(list);
	while (node != DLListEnd(list))
	{
		node = DLListNext(node);
		counter++;
	}
	
	return counter;
}

int DLListIsEmpty(const dl_list_t *list)
{
	return (DLListBegin(list) == DLListEnd(list));
}

dll_iter_t DLListFind(dll_iter_t begin, dll_iter_t end, 
					  const void *param, find_ptr ptr)
{
	dll_iter_t node = NULL;
	
	assert(begin);
	assert(end);
	assert(ptr);
	
	node = begin;
	
	for (node = begin ; node != end ; node = DLListNext(node))
	{
		if (1 == ptr(DLListGetData(node), param))
		{
			return node;
		}
	}
	
	return end;
}

int DLListForEach(dll_iter_t begin, dll_iter_t end, 
				  void *param, for_each_ptr ptr)
{
	dll_iter_t node = NULL;
	
	assert(begin);
	assert(end);
	assert(ptr);
	
	node = begin;
	for (node = begin ; node != end ; node = DLListNext(node))
	{
		if (1 == ptr(DLListGetData(node), param))
		{
			return 1;
		}
	}

	return 0;
}

dll_iter_t DLListBegin(const dl_list_t *list)
{
	assert(list);	
	
	return (list->begin->next);
}

dll_iter_t DLListEnd(const dl_list_t *list)
{
	assert(list);
		
	return (list->end);
}

dll_iter_t DLListNext(dll_iter_t iterator)
{
	assert(iterator);	
	
	return (iterator->next);
}

dll_iter_t DLListPrev(dll_iter_t iterator)
{
	assert(iterator);	

	return (iterator->prev);
}

void *DLListGetData(dll_iter_t iterator)
{
	assert(iterator);

	return (iterator->data);
}

int DLListIsSameIterator(dll_iter_t iterator1, dll_iter_t iterator2)
{
	assert(iterator1);
	assert(iterator2);

	return (iterator1 == iterator2);
}

static dll_node_t *DLListCreateNode(void *data, dll_node_t *next, dll_node_t *prev)
{
	dll_node_t *new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->data = data;
	new_node->next = next;
	new_node->prev = prev;
	
	return new_node;
}

static void DestroyNodes(dll_node_t *node)
{
	assert(node);	
	
	if (node->next != NULL)
	{
		DestroyNodes(node->next);
	}
	
	free(node);
	node = NULL;
}

