/************************************
*		Author: ChenR				  *
*		Reviewer: Tamir				  *
*		dllist						  *
*		7/11/2019					  *
*									  *
************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dllist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

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

/* Create node with data and next pointer */
static dll_node_t *CreateNode(void *data, dll_node_t *next);
/* Free all nodes in list */
void FreeAll(dll_node_t *node);
/* insert new_node after pos */
static dll_node_t *InsertAfter(dll_node_t *new_node, dll_node_t *pos);
/* insert new_node before pos */
static dll_node_t *InsertBefore(dll_node_t *new_node, dll_node_t *pos);
/* disconnect node from list (cutting it of the list without free) */
static void DisconnectIterator(dll_iter_t iterator);

dl_list_t *DLListCreate(void)
{
	int data = 0xDEADBEEF;
	
	dl_list_t *new_dll = (dl_list_t *)malloc(sizeof(dl_list_t));
	if (NULL == new_dll)
	{
		return NULL;
	}
	
	new_dll -> begin = CreateNode(&data, NULL);
	if (NULL == (new_dll -> begin))
	{
		new_dll -> begin = NULL;
		DLListDestroy(new_dll);
		
		return NULL;
	}
	
	new_dll -> end = CreateNode(&data, NULL);
	if (NULL == (new_dll -> end))
	{
		new_dll -> end = NULL;
		DLListDestroy(new_dll);
		
		return NULL;	
	}
	
	(new_dll -> begin -> next) = new_dll -> end;
	(new_dll -> begin -> prev) = NULL;
	(new_dll -> end -> next) = NULL;
	(new_dll -> end -> prev) = new_dll -> begin;
	
	return new_dll;
}

void DLListDestroy(dl_list_t *list)
{
	assert(list);
	
	FreeAll(list -> begin);
	list -> end = NULL;
	free(list);
}

dll_iter_t DLListPushFront(dl_list_t *list, void *data)
{	
	dll_node_t *new_node = CreateNode(data, NULL);
	if (NULL == new_node)
	{
		return (DLListEnd(list));
	}
	
	assert(list);
	
	return (InsertAfter(new_node, list -> begin));
}

dll_iter_t DLListPushBack(dl_list_t *list, void *data)
{
	dll_node_t *new_node = CreateNode(data, NULL);
	if (NULL == new_node)
	{
		return NULL;
	}
	
	assert(list);
	
	return (InsertBefore(new_node, list -> end));
}

dll_iter_t DLListInsert(void *data, dll_iter_t iterator, dl_list_t *list)
{
	dll_iter_t new_iter = CreateNode(data, NULL);
	if (NULL == new_iter)
	{
		return DLListEnd(list);
	}
	
	assert(list);
	assert(iterator);
		
	return(InsertBefore(new_iter, iterator));
}

dll_iter_t DLListRemove(dll_iter_t iterator)
{
	dll_iter_t returned_iter = NULL;
	
	assert(iterator);
	
	returned_iter = DLListNext(iterator);
	DisconnectIterator(iterator);
	iterator -> data = NULL;
	free(iterator);
/*	iterator = NULL;*/
	
	return 	returned_iter;
}

void *DLListPopFront(dl_list_t *list)
{
	void *res = NULL;
	dll_node_t *popped_node = list -> begin -> next;
	
	assert(list);
	
	res = DLListBegin(list) -> data;
	DisconnectIterator(popped_node);
	free(popped_node);
	
	return res;
}

void *DLListPopBack(dl_list_t *list)
{
	void *res = NULL;
	dll_node_t *popped_node = list -> end -> prev;
	
	assert(list);
	
	res = DLListPrev(DLListEnd(list)) -> data;
	DisconnectIterator(popped_node);
	free(popped_node);
	
	return res;
}

dll_iter_t DLListSplice(dll_iter_t s_begin, dll_iter_t s_end, dll_iter_t dest)
{
	dll_iter_t dest_next = NULL;
	dll_iter_t actual_end = NULL;
	
	
	assert(s_begin);
	assert(s_end);
	assert(dest);
	
	dest_next = DLListNext(dest);
	actual_end = s_end -> prev;
	(actual_end -> next) -> prev = (s_begin -> prev);
	(s_begin -> prev) -> next = (actual_end -> next);
	dest -> next = s_begin;
	s_begin -> prev = dest;
	dest_next -> prev = actual_end;
	actual_end -> next = dest_next;	
	
	return dest;
}

int DLListIsEmpty(const dl_list_t *list)
{
	return ((DLListNext(list -> begin) == (list -> end))
	&& (list -> begin == DLListPrev(list -> end)));
}

size_t DLListSize(const dl_list_t *list)
{
	size_t counter = 0;
	dll_iter_t node = NULL;
		
	for (node = DLListBegin(list); node != DLListEnd(list);
								   node = DLListNext(node))
	{
		++counter;
	}
	
	return counter;
}

int DLListIsSameIterator(dll_iter_t iterator1, dll_iter_t iterator2)
{
	assert(iterator1);
	assert(iterator2);
		
	return (iterator1 == iterator2);
}

void *DLListGetData(dll_iter_t iterator)
{
	assert(iterator);	

	return iterator -> data;
}

dll_iter_t DLListFind(dll_iter_t begin, dll_iter_t end, 
					  const void *param, find_ptr ptr)
{
	dll_iter_t node = NULL;
	
	assert(begin);
	assert(end);
	assert(ptr);
	
	for (node = begin; node != end; node = DLListNext(node))
	{
		if (ptr(node -> data, param))
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
	
	for (node = begin; node != end; node = DLListNext(node))
	{
		if (1 == ptr(node -> data, param))
		{
			return 1;
		}		
	}
	
	return 0;
}				  

dll_iter_t DLListBegin(const dl_list_t *list)
{
	assert(list);

	return (list -> begin -> next);
}

dll_iter_t DLListEnd(const dl_list_t *list)
{
	assert(list);
	
	return (list -> end);
}

dll_iter_t DLListNext(dll_iter_t iterator)
{
	assert(iterator);	

	return (iterator -> next);
}

dll_iter_t DLListPrev(dll_iter_t iterator)
{
	assert(iterator);
	
	return (iterator -> prev);
}

/*****************************************************************/
static void DisconnectIterator(dll_iter_t iterator)
{
	iterator -> next -> prev = DLListPrev(iterator);
	iterator -> prev -> next = DLListNext(iterator);	
	iterator -> next = NULL;
	iterator -> prev = NULL;	
}

static dll_node_t *CreateNode(void *data, dll_node_t *next)
{
	dll_node_t *new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	assert(NULL != data);
	
	new_node -> data = data;
	new_node -> next = next;
	
	return new_node;
}

void FreeAll(dll_node_t *node)
{
	dll_node_t *temp_node = NULL;
	
	assert(NULL != node);
	
	while (NULL != node)
	{
		temp_node = node;
		node = node -> next;
		free(temp_node);
	}
}

static dll_node_t *InsertAfter(dll_node_t *new_node, dll_node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> next = DLListNext(pos);
	pos -> next -> prev = new_node;
	pos -> next = new_node;
	new_node -> prev = pos;
	
	return new_node;
}

static dll_node_t *InsertBefore(dll_node_t *new_node, dll_node_t *pos)
{
	assert(NULL != new_node);
	assert(NULL != pos);
	
	new_node -> prev = DLListPrev(pos);
	pos -> prev -> next = new_node;
	pos -> prev = new_node;
	new_node -> next = pos;
	
	return new_node;
}
