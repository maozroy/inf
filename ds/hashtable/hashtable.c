 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	Jonathan
*						HASH					
*	Date: Thu Dec 26 16:27:29 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <sys/mman.h>
#include <math.h>
#include "../dllist/dllist.h"
#include "hashtable.h"


struct hash
{
	hash_func_t hash_func;
	is_match_t match_func;
	size_t num_of_buckets;
	dl_list_t **table;
};

static dll_iter_t GetElementIMP(hash_table_t *hash_table, const void *data);

hash_table_t *HashCreate(size_t num_of_buckets, hash_func_t hash_func,
 	                     is_match_t match_func)
{
	hash_table_t *hash_struct = NULL;
	dl_list_t **hash_table = NULL;
	size_t i = 0;
	
	assert(hash_func);
	assert(match_func);
	assert(num_of_buckets);
	
	hash_struct = (hash_table_t *)malloc(sizeof(hash_table_t));
	if (NULL == hash_struct)
	{
		return NULL;
	}
	hash_table = (dl_list_t **)malloc(sizeof(dl_list_t *) * num_of_buckets);
	if (NULL == hash_table)
	{
		return NULL;
	}
	
	hash_struct -> hash_func = hash_func;
	hash_struct -> match_func = match_func;
	hash_struct -> table = hash_table;
	hash_struct -> num_of_buckets = num_of_buckets;
	
	for (i = 0 ; i < num_of_buckets ; i++)
	{	
		(hash_table[i]) = DLListCreate();
	}
	
	return hash_struct;
}

void HashDestroy(hash_table_t *hash_table)
{
	size_t i = 0;
	for (i = 0 ; i < hash_table -> num_of_buckets ; i++)
	{
		DLListDestroy(hash_table->table[i]);
	}
	free(hash_table->table);
	free(hash_table);
}

int HashInsert(hash_table_t *hash_table, void *data)
{
	size_t key = 0;
	
	key = hash_table -> hash_func(data);
	
	if (DLListEnd(hash_table -> table[key]) == 
		DLListPushFront(hash_table -> table[key], data))
	{
		return 1;
	}
	return 0;
}

void HashRemove(hash_table_t *hash_table, const void *data)
{
	dll_iter_t iter_to_remove = NULL;
	
	iter_to_remove = GetElementIMP(hash_table, data);
	if (NULL == iter_to_remove)
	{
		return;
	}
	DLListRemove(iter_to_remove);
}


void *HashFind(const hash_table_t *hash_table, const void *data)
{
	dll_iter_t iter_to_get = NULL;
	
	iter_to_get = GetElementIMP((hash_table_t *)hash_table, data);
	return DLListGetData(iter_to_get);
	

}

static dll_iter_t GetElementIMP(hash_table_t *hash_table, const void *data)
{
	dll_iter_t iter_to_return = NULL;
	dl_list_t *bucket = NULL;
	size_t key = 0;
	
	key = hash_table -> hash_func(data);
	bucket = hash_table -> table[key];
	
	iter_to_return = DLListFind(DLListBegin(bucket), DLListEnd(bucket), 
					  data, hash_table -> match_func);
	if (iter_to_return == DLListEnd(bucket))
	{
		return NULL;
	}
	return iter_to_return;
}

int HashIsEmpty(const hash_table_t *hash_table)
{
	size_t i = 0;
	
	for (i = 0 ; i < hash_table -> num_of_buckets ; i++)
	{
		if (1 == DLListIsEmpty(hash_table->table[i]))
		{
			return 1;
		}
	}
	return 0;
}

size_t HashSize(const hash_table_t *hash_table)
{
	size_t i = 0;
	size_t result = 0;
	
	for (i = 0 ; i < hash_table -> num_of_buckets ; i++)
	{
		result += DLListSize(hash_table -> table[i]);
	}
	return result;
}

int HashForEach(hash_table_t *hash_table, action_func_t func,
				void *action_param)
{
	size_t i = 0;
	size_t result = 0;
	
	for (i = 0 ; (i < hash_table -> num_of_buckets) && result == 0 ; i++)
	{
		result = DLListForEach(DLListBegin(hash_table -> table[i]), 
				DLListEnd(hash_table -> table[i]), action_param, func);
	}
	return result;

}

double HashGetLoadFactor(const hash_table_t *hash_table)
{
	return HashSize(hash_table) / hash_table -> num_of_buckets;
}

double HashGetStandardDeviation(const hash_table_t *hash_table)
{
	double first_result = 0;
	size_t i = 0;
	double load_factor = HashGetLoadFactor(hash_table);
	
	for (i = 0 ; i < hash_table -> num_of_buckets ; i++)
	{
		first_result += pow((DLListSize(hash_table -> table[i]) - load_factor), 2);
	}
	first_result /= hash_table -> num_of_buckets;
	
	return sqrt(first_result);

}





























