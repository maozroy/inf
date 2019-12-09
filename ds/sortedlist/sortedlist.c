 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : Chen Rubin
*	Sorted (Doubly) List	
*	Date: Nov 13 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*assert*/
#include <stdio.h>

#include "../dllist/dllist.h"
#include "sortedlist.h"

/*side funcs*/
static int MyFind(const void *data, const void *param);
static int MyIsBeforeWrapper(const void *data, const void *param);

struct srt_list
{
	dl_list_t *dll;
	is_before_t is_before_ptr;
	void *is_before_param;
};

typedef struct wrap_for_find
{
	void *data;
	void *param;
	is_before_t is_before_ptr;
}my_wrap_for_find;

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
	assert(list);

	DLListDestroy(list->dll);
	free(list);
	list = NULL;
}

srt_iter_t SrtListInsert(void *data, srt_list_t *list)
{
	srt_iter_t new_srt_iter = {0};
	dll_node_t *node = NULL;
	my_wrap_for_find *wrapper = (my_wrap_for_find *)
								malloc(sizeof(my_wrap_for_find));

	wrapper->data = (void*)data;
	wrapper->param = list->is_before_param;
	wrapper->is_before_ptr = list->is_before_ptr;
	
		new_srt_iter = SrtListFindIf(SrtListBegin(list), SrtListEnd(list), 
					   wrapper, MyIsBeforeWrapper);

	node = DLListInsert(data, new_srt_iter.dll_iterator, list->dll);
	new_srt_iter.dll_iterator = node;
	free(wrapper);
	
	return new_srt_iter;
}

void SrtListMerge(srt_list_t *src_list, srt_list_t *dest_list)
{
	srt_iter_t src_runner = {0};
	srt_iter_t dest_runner = {0};
	is_before_t is_before = dest_list->is_before_ptr;
	void *before_param = dest_list->is_before_param;
	
	while(!SrtListIsEmpty(src_list))
	{
		src_runner = SrtListBegin(src_list);
		dest_runner = SrtListBegin(dest_list);
	
		while (!(is_before(SrtListGetData(src_runner), 
		SrtListGetData(dest_runner),before_param)) && !SrtListIsSameIterator
		(dest_runner, SrtListEnd(dest_list)))
		{
			if (SrtListIsSameIterator
				(dest_runner, SrtListPrev(SrtListEnd(dest_list))))
				{
					src_runner = SrtListNext(src_runner);
					DLListSplice(SrtListBegin(src_list).dll_iterator, 
					src_runner.dll_iterator, dest_runner.dll_iterator);
					
					return;
				}
			dest_runner = SrtListNext(dest_runner);
		}
		while (is_before(SrtListGetData(src_runner), SrtListGetData(dest_runner)
				, before_param) && !SrtListIsSameIterator
				(src_runner, SrtListEnd(src_list)))
		{
			src_runner = SrtListNext(src_runner);
		}
		
		DLListSplice(SrtListBegin(src_list).dll_iterator, 
					src_runner.dll_iterator, 
					SrtListPrev(dest_runner).dll_iterator);
		}
}

srt_iter_t SrtListRemove(srt_iter_t iterator)
{
	srt_iter_t new_srt_iterator = {0};
	
	assert(iterator.dll_iterator);
	
	new_srt_iterator.dll_iterator = DLListRemove(iterator.dll_iterator); 
	
	return (new_srt_iterator);
}

srt_iter_t SrtListBegin(srt_list_t *list)
{
	srt_iter_t new_srt_iterator = {0};
	
	assert(list);
	
	new_srt_iterator.dll_iterator = DLListBegin(list->dll); 
	
	return (new_srt_iterator);
}

size_t SrtListSize(const srt_list_t *list)
{
	assert(list);

	return DLListSize(list->dll);
}

srt_iter_t SrtListEnd(srt_list_t *list)
{
	srt_iter_t temp_iter = {0};
	
	assert(list);
	
	temp_iter.dll_iterator = DLListEnd(list->dll);
	
	return temp_iter;
}

srt_iter_t SrtListNext(srt_iter_t iterator)
{
	srt_iter_t srt_iter_holder;
	
	assert(iterator.dll_iterator);
	
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
	assert(list);

	return DLListIsEmpty(list->dll);
}
srt_iter_t SrtListFind(srt_iter_t begin, srt_iter_t end, 
					   const void *data, srt_list_t *list)
{	
	srt_iter_t result = {0};
	my_wrap_for_find *wrapper = (my_wrap_for_find *)
								malloc(sizeof(my_wrap_for_find));
	if(NULL == wrapper)
	{
		return end;
	}

	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(list);
	
	wrapper->data = (void*)data;
	wrapper->param = list->is_before_param;
	wrapper->is_before_ptr = list->is_before_ptr;
	result.dll_iterator = DLListFind(begin.dll_iterator,
						 end.dll_iterator, (void *)wrapper, MyFind);
	free(wrapper);
	wrapper = NULL;
	
	return result;
}

int SrtListForEach(srt_iter_t begin, srt_iter_t end, 
					void *param, action_func_t ptr)
{
	int result = 0;
	result = DLListForEach(begin.dll_iterator,
			end.dll_iterator, param, ptr);
	
	return result;
}

srt_iter_t SrtListFindIf(srt_iter_t begin, srt_iter_t end, 
						 const void *param, is_match_func_t ptr)
{
	srt_iter_t result = {0};

	assert(ptr);
	
	result.dll_iterator = DLListFind(begin.dll_iterator,
							end.dll_iterator, param, ptr);
	
	return result;
}

static int MyIsBeforeWrapper(const void *data, const void *param)
{
	my_wrap_for_find *ze_struct = (my_wrap_for_find *)param;
	void *node_data = ze_struct->data;
	void *is_before_param = ze_struct->param;
	is_before_t is_before = ze_struct->is_before_ptr;
	
	 return is_before(node_data, data, is_before_param);
}

static int MyFind(const void *data, const void *param)
{
	my_wrap_for_find *ze_struct = (my_wrap_for_find *)param;
	void *node_data = ze_struct->data;
	void *is_before_param = ze_struct->param;
	is_before_t is_before = ze_struct->is_before_ptr;
	
	if((is_before(node_data, data, is_before_param)) == 
		(is_before(data, node_data, is_before_param)))
	{
		return 1;
	}
	
	return 0;
}
