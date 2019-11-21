/************************************
*		Author: ChenR				  *
*		Reviewer: Sharon			  *
*		sortedlist					  *
*		7/11/2019					  *
*									  *
************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
	
#include "dllist.h"
#include "sortedlist.h"
#include "MyUtils.h" /* MAX2,MIN2 */

int MyFindAction(const void *data, const void *wrap);

struct srt_list
{
	dl_list_t *dll;
	is_before_t is_before_ptr;
	void *is_before_param;
};

typedef struct wrapper3
{
	const void *data;
	void *is_before_param;
	is_before_t match_ptr;
} structWrapperFind;

srt_list_t *SrtListCreate(void *param, is_before_t ptr)
{
	srt_list_t *new_srtlist = (srt_list_t *)malloc(sizeof(srt_list_t));
	if (NULL == new_srtlist)
	{
		return NULL;
	}
	
	new_srtlist -> dll = DLListCreate();
	if (NULL == new_srtlist -> dll)
	{
		free(new_srtlist);
		
		return NULL;
	}
	
	assert(ptr);
	
	new_srtlist -> is_before_ptr = ptr;
	new_srtlist -> is_before_param = param;
	
	return new_srtlist;
}

void SrtListDestroy(srt_list_t *list)
{
	assert(list);
	
	DLListDestroy(list -> dll);
	list -> is_before_ptr = NULL;
	list -> is_before_param = NULL;
	free(list);
	list = NULL;
}

srt_iter_t SrtListInsert(void *data, srt_list_t *list)
{
	dll_node_t *node = NULL;
	srt_iter_t srt_iter = {0};
	
	assert(list);
	
	for (node = DLListBegin(list -> dll);
		 node != DLListEnd(list -> dll);
		 node = DLListNext(node))
	{
		if (1 == (list -> is_before_ptr(data, DLListGetData(node), 
				  list -> is_before_param)))
		{
			break;
		}
	}
	
	srt_iter.dll_iterator = DLListInsert(data, node, list -> dll);
	
	return srt_iter;
}

srt_iter_t SrtListBegin(srt_list_t *list)
{
	srt_iter_t srt_iter = {0};
	
	assert(list);
	
	srt_iter.dll_iterator = DLListBegin(list -> dll);
	
	return srt_iter;
}

srt_iter_t SrtListEnd(srt_list_t *list)
{
	srt_iter_t srt_iter = {0};
	
	assert(list);
	
	srt_iter.dll_iterator = DLListEnd(list -> dll);
	
	return srt_iter;
}

srt_iter_t SrtListNext(srt_iter_t iterator)
{
	srt_iter_t srt_iter = {0};
	
	assert(iterator.dll_iterator);
	
	srt_iter.dll_iterator = DLListNext(iterator.dll_iterator);
	
	return srt_iter;
}

srt_iter_t SrtListPrev(srt_iter_t iterator)
{
	srt_iter_t srt_iter = {0};
	
	assert(iterator.dll_iterator);
	
	srt_iter.dll_iterator = DLListPrev(iterator.dll_iterator);
	
	return srt_iter;
}

void *SrtListGetData(srt_iter_t iterator)
{
	assert(iterator.dll_iterator);
	
	return (DLListGetData(iterator.dll_iterator));
}

int SrtListIsSameIterator(srt_iter_t iterator1, srt_iter_t iterator2)
{
	assert(iterator1.dll_iterator);
	assert(iterator2.dll_iterator);
	
	return (iterator1.dll_iterator == iterator2.dll_iterator);
}

void *SrtListPopFront(srt_list_t *list)
{
	assert(list);
	
	return (DLListPopFront(list -> dll));
}

void *SrtListPopBack(srt_list_t *list)
{
	assert(list);
	
	return (DLListPopBack(list -> dll));
}

size_t SrtListSize(const srt_list_t *list)
{
	assert(list);
	
	return (DLListSize(list -> dll));
}

srt_iter_t SrtListRemove(srt_iter_t iterator)
{
	assert(iterator.dll_iterator);
		
	iterator.dll_iterator = DLListRemove(iterator.dll_iterator);
	
	return iterator;
}

int SrtListIsEmpty(const srt_list_t *list)
{
	assert(list);
	
	return (DLListIsEmpty(list -> dll));
}

void SrtListMerge(srt_list_t *src_list, srt_list_t *dest_list)
{
	srt_iter_t srt_iter = {0};
	
	assert(src_list);
	assert(dest_list);
	
	for (srt_iter = SrtListBegin(src_list); 
		 !SrtListIsSameIterator(srt_iter, SrtListEnd(src_list));
		 srt_iter = SrtListNext(srt_iter))
	{
		SrtListInsert(SrtListGetData(srt_iter), dest_list);
	}
	
	SrtListDestroy(src_list);
}

int SrtListForEach(srt_iter_t begin, srt_iter_t end, void *param, 
				   action_func_t ptr)
{
	int status = 1;
	
	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(ptr);

	status = (DLListForEach(begin.dll_iterator, end.dll_iterator, param, ptr));

	return status;			  
}

srt_iter_t SrtListFindIf(srt_iter_t begin, srt_iter_t end, 
						 const void *param, is_match_func_t ptr)
{
	srt_iter_t srt_iter = {0};
	
	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(ptr);
	
	srt_iter.dll_iterator = DLListFind(begin.dll_iterator, end.dll_iterator, 
					  param, ptr);	  
					  
	return srt_iter;				  
}

srt_iter_t SrtListFind(srt_iter_t begin, srt_iter_t end, 
					   const void *data, srt_list_t *list)
{
	srt_iter_t srt_iter = {0};
	structWrapperFind *wrap = 
	(structWrapperFind *)malloc(sizeof(structWrapperFind));
	if (NULL == wrap)
	{
		return end;
	}
	
	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(list);
	
	wrap -> match_ptr = list -> is_before_ptr;
	wrap -> is_before_param = list -> is_before_param;
	wrap -> data = data;
	
	srt_iter.dll_iterator = DLListFind(begin.dll_iterator, end.dll_iterator,
					  (structWrapperFind *)wrap, MyFindAction);
	free(wrap);				  
					  
	return srt_iter;	
}					   

/**************************************************/

int MyFindAction(const void *data, const void *wrap)
{
	int status1 = 0;
	int status2 = 0;
	structWrapperFind *my_wrapper = (structWrapperFind *)wrap;

	status1 = my_wrapper -> match_ptr(data, my_wrapper -> data, 
			  my_wrapper -> is_before_param);
	
	status2 = my_wrapper -> match_ptr(my_wrapper -> data, data, 
			  my_wrapper -> is_before_param);
	
	return (status1 == status2);
}
