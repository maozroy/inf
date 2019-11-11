 /********************************************
*	Author : 			
*	Reviewer : 	
*	dllist					
*	Date: Mon Nov 11 15:56:38 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <string.h> /*memcpy*/
#include <assert.h> /*assert*/

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

static dll_node_t *DLListCreateNode(void *data, dll_node_t *next, dll_node_t *prev);
static void DestroyNodes(dll_node_t *node);
void PrintIntList(dl_list_t *mylist);


dl_list_t *DLListCreate(void)
{
	dll_node_t *dummy_begin = NULL;
	dll_node_t *dummy_end = NULL;
	dl_list_t *new_list = malloc(sizeof(dl_list_t));
	if (NULL == new_list)
	{
		return NULL;
	}
	
	dummy_begin = DLListCreateNode("0xDEADBEEF",NULL,NULL);
	if (NULL == dummy_begin)
	{
		free(new_list);
		
		return NULL;
	}
	new_list->begin = dummy_begin;
	dummy_end = DLListCreateNode("0xDEADBEEF", NULL ,dummy_begin);
	if (NULL == dummy_end)
	{
		free(new_list);
		free(dummy_begin);
		
		return NULL;
	}
	new_list->end = dummy_end;
	new_list->begin->next = dummy_end;
	
	return new_list;
}


void DLListDestroy(dl_list_t *list)
{
	DestroyNodes(list->begin);
	free(list);
}

dll_iter_t DLListPushFront(dl_list_t *list, void *data)
{
	dll_node_t *new_node = NULL;
	new_node = DLListCreateNode(data, NULL, NULL);
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->next = list->begin->next;
	new_node->prev = list->begin;
	(list->begin->next)->prev = new_node;
	list->begin->next = new_node;

	return new_node;

}

dll_iter_t DLListPushBack(dl_list_t *list, void *data)
{
	dll_node_t *new_node = NULL;
	new_node = DLListCreateNode(data, NULL, NULL);
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->next = list->end;
	new_node->prev = list->end->prev;
	(list->end->prev)->next = new_node;
	list->end->prev = new_node;

	return new_node;
}


dll_iter_t DLListInsert(void *data, dll_iter_t iterator, dl_list_t *list)
{
	dll_node_t *new_node = NULL;
	new_node = DLListCreateNode(data, NULL, NULL);
	(void)list;
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->next = iterator;
	new_node->prev = iterator->prev;
	iterator->prev->next = new_node;
	iterator->prev = new_node;
	
	return new_node;	
}

void *DLListPopFront(dl_list_t *list)
{
	void *ptr = DLListGetData(DLListBegin(list));
	dll_node_t *node_to_remove = NULL;
	
	node_to_remove = list->begin->next;
	(list->begin->next->next)->prev = list->begin;
	list->begin->next = list->begin->next->next;
	free(node_to_remove);
	
	return ptr;
}

void *DLListPopBack(dl_list_t *list)
{
	void *ptr = DLListGetData(DLListEnd(list));
	dll_node_t *node_to_remove = NULL;
	
	node_to_remove = list->end->prev;
	list->end->prev->prev->next = list->end;
	list->end->prev = list->end->prev->prev;
	free(node_to_remove);
	
	return ptr;
}

dll_iter_t DLListRemove(dll_iter_t iterator)
{
	dll_node_t *prev_node = iterator->prev;
	iterator->prev->next = iterator->next;
	iterator->next->prev = prev_node;
	free(iterator);
	
	return prev_node->next;
}

dll_iter_t DLListSplice(dll_iter_t s_begin, dll_iter_t s_end, dll_iter_t dest)
{
	s_begin->prev = dest;
	s_end->next = dest->next;
	dest->next->prev = s_end;
	dest->next = s_begin;
	
	return dest;
}


size_t DLListSize(const dl_list_t *list)
{
	dll_node_t *node = NULL;
	size_t counter = 0;
	
	node = list->begin;
	while (node->next != NULL)
	{
		node = node->next;
		counter++;
	}
	
	return counter;

}

int DLListIsEmpty(const dl_list_t *list)
{
	return (list->begin->next == list->end);
}

dll_iter_t DLListFind(dll_iter_t begin, dll_iter_t end, 
					  const void *param, find_ptr ptr)
{
	dll_iter_t node = begin;
	
	assert(node);
	assert(ptr);
	assert(param);
	
	for (node = begin ; node != DLListNext(end) ; node = DLListNext(node))
	{
		if (ptr(node, param) == 1)
		{
			return node;
		}
	}
	
	return NULL;
}

int DLListForEach(dll_iter_t begin, dll_iter_t end, 
				  void *param, for_each_ptr ptr)
{
	dll_iter_t node = begin;
	for (node = begin ; node != DLListNext(end) ; node = DLListNext(node))
	{
		if(ptr(node, param) == 1)
		{
			return 1;
		}
	}

	return 0;

}

dll_iter_t DLListBegin(const dl_list_t *list)
{
	return (list->begin->next);
}


dll_iter_t DLListEnd(const dl_list_t *list)
{
 return (list->end->prev);
}


dll_iter_t DLListNext(dll_iter_t iterator)
{
	return (iterator->next);
}

dll_iter_t DLListPrev(dll_iter_t iterator)
{
	return (iterator->prev);
}

void *DLListGetData(dll_iter_t iterator)
{
	return (iterator->data);
}

int DLListIsSameIterator(dll_iter_t iterator1, dll_iter_t iterator2)
{
	return (iterator1 == iterator2);
}

void PrintIntList(dl_list_t *mylist)
{
	size_t size = DLListSize(mylist);
	dll_node_t *node = mylist->begin->next;
	
	while(size > 1)
	{
		printf("%d\n",*(int*)DLListGetData(node));
		node = node->next;
		--size;
	}
}

static dll_node_t *DLListCreateNode(void *data, dll_node_t *next, dll_node_t *prev)
{
	dll_node_t *new_node = malloc(sizeof(dll_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	assert(data);
	
	new_node->data = data;
	new_node->next = next;
	new_node->prev = prev;
	
	return new_node;
}

static void DestroyNodes(dll_node_t *node)
{
	if(node->next != NULL)
	{
		DestroyNodes(node->next);
	}
	free(node);
}
