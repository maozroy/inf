/****************************************************************************
*                                                                           *
*                                Tamir Dayan                                *
*                              OL79 - 13/11/19                              *
*                                                                           *	
*                                Sorted List                                *
*                           Reviewer: Maoz Roytman                          *
*****************************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "sortedlist.h" /* sortedlist.h */
#include "dllist.h" /* dllist.h */

#define LIST list->dll
#define ITERATOR iterator.dll_iterator

static int Find(const void *data, const void *param);
									
struct srt_list
{
	dl_list_t *dll;
	is_before_t is_before_ptr;
	void *is_before_param;
};

typedef struct find_param
{
	void *param;
    const void *data;
	is_before_t is_before_ptr;
}find_wrapper;

srt_list_t *SrtListCreate(void *param, is_before_t ptr)
{
	srt_list_t *list = NULL;
	
	assert(ptr);
	    
	list = (srt_list_t*)malloc(sizeof(srt_list_t));
	if (NULL == list)
    {
        return NULL;
    }
    
    LIST = DLListCreate();
    if (NULL == LIST)
    {
    	free(list);
    	list = NULL;
        return NULL;
    }
    
	list->is_before_ptr = ptr;
	list->is_before_param = param;
    
	return list;
}

void SrtListDestroy(srt_list_t *list)
{
	assert(list);
	
	DLListDestroy(LIST);
	free(list);
	list = NULL;
}

size_t SrtListSize(const srt_list_t *list)
{
	assert(list);
	
	return (DLListSize(LIST));
}

int SrtListIsEmpty(const srt_list_t *list)
{
	assert(list);
	
	return (DLListIsEmpty(LIST));
}

srt_iter_t SrtListInsert(void *data, srt_list_t *list)
{
	srt_iter_t iterator = {0};
	dll_node_t *node = NULL;
	
	assert(list);
	
	for (node = DLListBegin(LIST); node != DLListEnd(LIST); 
		node = DLListNext(node))
	{
		if (1 == list->is_before_ptr(data, DLListGetData(node), NULL))
		{
			node = DLListInsert(data, node, LIST);
			ITERATOR = node;
			
			return iterator;
		}
	}		 

	node = DLListPushBack(LIST, data);
	ITERATOR = node;
				
	return iterator;
}

srt_iter_t SrtListRemove(srt_iter_t iterator)
{
	assert(ITERATOR);
	
	ITERATOR = DLListRemove(ITERATOR);
	
	return iterator;
}

void *SrtListPopFront(srt_list_t *list)
{
	assert(list);
	
	return (DLListPopFront(LIST));
}

void *SrtListPopBack(srt_list_t *list)
{
	assert(list);
	
	return (DLListPopBack(LIST));
}

static int Find(const void *data, const void *param)
{
	const find_wrapper *wrapper = NULL;
    const void *src_data;
    is_before_t is_before;
    int result1 = 0;
    int result2 = 0;
	
	assert(param);
	
	wrapper = (void*)param;
    src_data = (const void*)data;
    is_before = wrapper->is_before_ptr; 

	result1 = is_before(wrapper->data, src_data, wrapper->param);
	result2 = is_before(src_data, wrapper->data, wrapper->param);

	return (result1 == result2);
}

srt_iter_t SrtListFind(srt_iter_t begin, srt_iter_t end, 
					   const void *data, srt_list_t *list)
{
	srt_iter_t result = {0};
	find_ptr ptr = Find;
	find_wrapper *wrapper = (find_wrapper*)malloc(sizeof(find_wrapper));
    if (NULL == wrapper)
    {
        free(wrapper);
        return end;
    }

	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(list);

	wrapper->data = data;	
	wrapper->param = list->is_before_param;
	wrapper->is_before_ptr = list->is_before_ptr;
	
	result.dll_iterator = DLListFind(begin.dll_iterator,
						 end.dll_iterator, wrapper, ptr);
	free(wrapper);
	
	return result;
}

srt_iter_t SrtListFindIf(srt_iter_t begin, srt_iter_t end, 
						 const void *param, is_match_func_t match_func)
{
	srt_iter_t iterator = {0};

	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(match_func);
	
	iterator.dll_iterator = DLListFind(begin.dll_iterator, end.dll_iterator, 
                                       param, match_func);
	
	return iterator;				 
}

int SrtListForEach(srt_iter_t begin, srt_iter_t end, void *param, 
				   action_func_t action_ptr)
{
	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(action_ptr);
	
	return (DLListForEach(begin.dll_iterator, end.dll_iterator, param,
						   action_ptr));
}

void SrtListMerge(srt_list_t *src_list, srt_list_t *dest_list)
{
	dll_node_t *node = NULL;

	assert(src_list);
	assert(dest_list);	
	
	for (node = DLListBegin(src_list->dll); node != DLListEnd(src_list->dll); 
		node = DLListNext(node))
	{
		SrtListInsert(DLListGetData(node), dest_list);		
	}		 

	SrtListDestroy(src_list);
}

srt_iter_t SrtListBegin(srt_list_t *list)
{
	srt_iter_t begin = {0};
	
	assert(list);
	
	begin.dll_iterator = DLListBegin(LIST);
	
	return begin;
}

srt_iter_t SrtListEnd(srt_list_t *list)
{
	srt_iter_t end = {0};
	
	assert(list);
	
	end.dll_iterator = DLListEnd(LIST);
	
	return end;
}

srt_iter_t SrtListNext(srt_iter_t iterator)
{
	assert(ITERATOR);
	
	ITERATOR = DLListNext(ITERATOR);
	
	return iterator;
}

srt_iter_t SrtListPrev(srt_iter_t iterator)
{
	assert(ITERATOR);
	
	ITERATOR = DLListPrev(ITERATOR);
	
	return iterator;
}

void *SrtListGetData(srt_iter_t iterator)
{
	assert(ITERATOR);
	
	return (DLListGetData(ITERATOR));
}

int SrtListIsSameIterator(srt_iter_t iterator1, srt_iter_t iterator2)
{
	assert(iterator1.dll_iterator);
	assert(iterator2.dll_iterator);
	
	return (DLListIsSameIterator(iterator1.dll_iterator, 
								 iterator2.dll_iterator));
}
