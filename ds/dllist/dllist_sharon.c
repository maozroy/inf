/*****************************************/
/* OL79                                  */
/* ds_Doubly_Linked_List                 */
/* 12/11/19                              */
/* Author- Sharon Rottner                */
/* Reviewer- Maoz Roytman                */
/*****************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "dllist.h"

#define BEGIN_DUMMY (list->begin)
#define END_DUMMY (list->end)

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

static dll_node_t *DListCreateNode(void *data, dll_node_t *next, dll_node_t *prev)
{
	dll_node_t *ptr_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	if (NULL == ptr_node)
	{
		return NULL;
	}

	assert(data);

	ptr_node->data = data;
	ptr_node->next = next; 
	ptr_node->prev = prev;	

	return ptr_node;
}

dl_list_t *DLListCreate(void)
{
	dl_list_t *list = (dl_list_t *)malloc(sizeof(dl_list_t));
	if (NULL == list)
	{
		return NULL;
	}		
	
	BEGIN_DUMMY = DListCreateNode("0xDEADBEAF", NULL, NULL);
	if (NULL == BEGIN_DUMMY)
	{
		free(list);
		list = NULL;

		return NULL;
	}
	
	END_DUMMY = DListCreateNode("0xDEADBEAF", NULL, BEGIN_DUMMY);
	if (NULL == END_DUMMY)
	{
		free(list);
		list = NULL;
		free(BEGIN_DUMMY);
		BEGIN_DUMMY = NULL;

		return NULL;
	}

	BEGIN_DUMMY->next = END_DUMMY;
	
	return list; 
}

void DLListDestroy(dl_list_t *list)
{
	dll_node_t *next_to_free = NULL;
	dll_node_t *node = NULL;
	assert(list);

	for (node = DLListPrev(DLListBegin(list)); node != DLListEnd(list);
        node = next_to_free)
	{
		next_to_free = DLListNext(node);
		free(node);
		node = NULL;
	}

	free(END_DUMMY);
	END_DUMMY = NULL;
	free(list);
	list = NULL;
}

dll_iter_t DLListPushBack(dl_list_t *list, void *data)
{
	dll_node_t *new_node = NULL;
	
	assert(data);
	assert(list);
	
	new_node = DListCreateNode(data, END_DUMMY, END_DUMMY->prev);
	END_DUMMY->prev = new_node;
	(new_node->prev)->next = new_node;
	
	return new_node;
}

dll_iter_t DLListPushFront(dl_list_t *list, void *data)
{
	dll_node_t *new_node = NULL;
	
	assert(data);
	assert(list);
	
	new_node = DListCreateNode(data, BEGIN_DUMMY->next, BEGIN_DUMMY);
	(BEGIN_DUMMY->next)->prev = new_node;
	BEGIN_DUMMY->next = new_node;
	
	return new_node;
}

dll_iter_t DLListInsert(void *data, dll_iter_t iterator, dl_list_t *list)
{
	dll_node_t *new_node = NULL;
	
	assert(data);
	assert(list);
	assert(iterator);
	
	new_node = DListCreateNode(data, iterator, iterator->prev);
	(iterator->prev)->next = new_node;
	iterator->prev = new_node;
	
	return new_node;
}

void *DLListPopFront(dl_list_t *list)
{
	dll_node_t *node_to_free = NULL;
	void *save_data = NULL;

	assert(list);
	
	node_to_free = BEGIN_DUMMY->next;
	((BEGIN_DUMMY->next)->next)->prev = BEGIN_DUMMY;
	BEGIN_DUMMY->next = (BEGIN_DUMMY->next)->next;
	save_data = node_to_free->data;
	free(node_to_free);
	node_to_free = NULL;

	
	return save_data;
}

void *DLListPopBack(dl_list_t *list)
{
	dll_node_t *node_to_free = NULL;
	void *save_data = NULL;

	assert(list);
	
	node_to_free = END_DUMMY->prev;
	((END_DUMMY->prev)->prev)->next = END_DUMMY;
	END_DUMMY->prev = (END_DUMMY->prev)->prev;
	save_data = node_to_free->data;
	free(node_to_free);
	node_to_free = NULL;
	
	return save_data; 
}

dll_iter_t DLListRemove(dll_iter_t iterator)
{
	dll_node_t *node_to_free = iterator;
	dll_node_t *node_to_return = NULL;

	assert(iterator);
	
	node_to_return = iterator->next;
	(iterator->prev)->next = (iterator->next);
	(iterator->next)->prev = iterator->prev;
	free(node_to_free);
	node_to_free = NULL;

	return node_to_return; 
}

dll_iter_t DLListSplice(dll_iter_t s_begin, dll_iter_t s_end, dll_iter_t dest)
{
	dll_node_t *node_to_save = NULL;

	assert(s_begin);
	assert(s_end);
	assert(dest);

	node_to_save = dest->next;
	(s_begin->prev)->next = s_end->next;
	(s_end->next)->prev = s_begin->prev;

	s_begin->prev = dest;
	dest->next = s_begin;
	s_end->next = node_to_save;
	node_to_save->prev = s_end;
	
	return dest;
}

size_t DLListSize(const dl_list_t *list)
{
	size_t size = 0;
	dll_node_t *node = NULL;

	assert(list);

	for (node = DLListBegin(list); node != DLListEnd(list);
 	     node=DLListNext(node))
	{
		++size;
	}
	
	return size;	
}

int DLListIsEmpty(const dl_list_t *list)
{
	return ((list->begin)->next == END_DUMMY);
}

dll_iter_t DLListBegin(const dl_list_t *list)
{
	assert(list);
	
	return ((list->begin)->next);
}

dll_iter_t DLListEnd(const dl_list_t *list)
{
	assert(list);
	
	return END_DUMMY;
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

dll_iter_t DLListFind(dll_iter_t begin, dll_iter_t end,const void *param,
find_ptr ptr)
{
	dll_node_t *node = NULL;

	assert(ptr);
	assert(param);
	assert(end);
	assert(begin);

	for (node = begin; node != DLListNext(end);
 	     node=DLListNext(node))
	{
		if (ptr(node, param))
		{
			return node;
		}
	}
	
	return NULL;				
}

int DLListForEach(dll_iter_t begin, dll_iter_t end, void *param,
for_each_ptr ptr)
{
	dll_node_t *node = NULL;

	assert(ptr);
	assert(param);
	assert(begin);
	assert(end);

	for (node = begin; node != DLListNext(end);
 	     node=DLListNext(node))
	{

		if (ptr(node, param))
		{
			return 1;
		}
	}
	
	return 0;				
}

