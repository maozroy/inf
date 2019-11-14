 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : Chen Rubin
*	dllist	
*	Date: Nov 13 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*assert*/
#include <stdio.h> /*printing*/

#include "dllist.h"
#include "sortedlist.h"

struct srt_list
{
	dl_list_t *dll;
	is_before_t is_before_ptr;
	void *is_before_param;
};

typedef struct wrap_struct
{
	is_before_t ptr;
	void *param;
}my_wrap_struct;



srt_list_t *SrtListCreate(void *param, is_before_t ptr)
{
	srt_list_t *new_sortlist = NULL;
	dl_list_t *new_dl_list = NULL;
	
	assert(ptr);
	
	new_dl_list = DLListCreate();
	if (NULL == new_dl_list)
	{
		return NULL;
	}
	
	new_sortlist = (srt_list_t *)malloc(sizeof(srt_list_t));
	if (NULL == new_sortlist)
	{
		free(new_dl_list);
		return NULL;
	}
	
	new_sortlist->is_before_ptr = ptr;
	new_sortlist->dll = new_dl_list;
	new_sortlist->is_before_param = param;
	
	return new_sortlist;
	
}

void SrtListDestroy(srt_list_t *list)
{
	DLListDestroy(list->dll);
	free(list);
}

srt_iter_t SrtListInsert(void *data, srt_list_t *list)
{
	srt_iter_t new_srt_iter;
	dll_node_t *node = DLListBegin(list->dll);
	
	for( ; node != DLListEnd(list->dll) ; node = DLListNext(node))
	{
		if (list->is_before_ptr(DLListGetData(node), 
			data, list->is_before_param) == 1)
		{
			break;
		}
	}
	node = DLListInsert(data, node, list->dll);
	new_srt_iter.dll_iterator = node;
	
	return new_srt_iter;
}

void SrtListMerge(srt_list_t *src_list, srt_list_t *dest_list)
{
	dll_node_t *src_node = DLListBegin(src_list->dll);
	for( ; src_node != DLListEnd(src_list->dll) ; 
			src_node = DLListNext(src_node))
		{
			SrtListInsert(DLListGetData(src_node), dest_list);
		}
	SrtListDestroy(src_list);
}


srt_iter_t SrtListRemove(srt_iter_t iterator)
{
	srt_iter_t new_srt_iterator;
	new_srt_iterator.dll_iterator = DLListRemove(iterator.dll_iterator); 
	
	return (new_srt_iterator);
}

srt_iter_t SrtListBegin(srt_list_t *list)
{
	srt_iter_t new_srt_iterator;
	new_srt_iterator.dll_iterator = DLListBegin(list->dll); 
	
	return (new_srt_iterator);
}

size_t SrtListSize(const srt_list_t *list)
{
	return DLListSize(list->dll);
}

srt_iter_t SrtListEnd(srt_list_t *list)
{
	srt_iter_t temp_list;
	
	temp_list.dll_iterator = DLListEnd(list->dll);
	
	return temp_list;
}

srt_iter_t SrtListNext(srt_iter_t iterator)
{
	srt_iter_t srt_iter_holder;
	
	srt_iter_holder.dll_iterator = DLListNext(iterator.dll_iterator);
	
	return srt_iter_holder;
}


srt_iter_t SrtListPrev(srt_iter_t iterator)
{
	srt_iter_t srt_iter_holder;
	
	assert(iterator.dll_iterator);
	
	srt_iter_holder.dll_iterator = DLListPrev(iterator.dll_iterator);
	
	return srt_iter_holder;
}


void *SrtListGetData(srt_iter_t iterator)
{
	assert(iterator.dll_iterator);

	return DLListGetData(iterator.dll_iterator);
}

void *SrtListPopFront(srt_list_t *list)
{
	assert(list);
	
	return DLListPopFront(list->dll);
}

void *SrtListPopBack(srt_list_t *list)
{
	assert(list);
	
	return DLListPopBack(list->dll);
}
int SrtListIsSameIterator(srt_iter_t iterator1, srt_iter_t iterator2)
{
	return(iterator1.dll_iterator == iterator2.dll_iterator);
}

int SrtListIsEmpty(const srt_list_t *list)
{
	return DLListIsEmpty(list->dll);
}
srt_iter_t SrtListFind(srt_iter_t begin, srt_iter_t end, 
					   const void *data, srt_list_t *list)
{	
	void *wrap_ptr = NULL;
	my_wrap_struct *wrapper = (my_wrap_struct *)malloc(sizeof(my_wrap_struct));
	wrapper->ptr = list->is_before_ptr;
	wrapper->param = (void*)data;
	wrap_ptr = wrapper;
	DLListFind(begin.dll_iterator, end.dll_iterator, MyFind);
	
	

}

int MyFind(const dll_node_t *node, const void *param)
{
	my_wrap_struct *ze_struct = (my_wrap_struct *)param;
	ze_struct->

}

/*
DLListFind(dll_iter_t begin, dll_iter_t end, 
					  const void *param, find_ptr ptr);
					  (*find_ptr)(const dll_node_t *node, const void *param);
					  (*is_match_func_t)(const void *data, const void *param);


int my_find_ptr(dll_node_t *node, void *param)
{
	
}

srt_iter_t SrtListFindIf(srt_iter_t begin, srt_iter_t end, 
						 const void *param, is_match_func_t ptr)
	{
		srt_iter_t my_iter = begin;
		srt_list_t *temp_list = SrtListCreate(param, ptr);
		
		
		while(!(SrtListIsSameIterator(my_iter, end)))
		{
			if (1 == ptr(SrtListGetData(my_iter), param))
			{
				return my_iter;
			}
			my_iter = SrtListNext(my_iter);
		}
		
		return end;
	}	
	*/
int SrtListForEach(srt_iter_t begin, srt_iter_t end, 
	void *param, action_func_t ptr)
{
	srt_iter_t my_iter = begin;
}

/*


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
		if (1 == ptr(node, param))
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
*/
