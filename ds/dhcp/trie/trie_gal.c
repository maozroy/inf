/****************************************************************************
                                                                           
			                 TRIE	                        
                             OL79 - 5/1/20                              
                                                                           
                             Writer:    Gal Salemon                       
                             Reviewer:  Maoz                   
*****************************************************************************/
#include <stdlib.h> 
#include <assert.h>
#include <math.h> 

#include "trie.h"

#define ZERO (0)
#define ONE (1)
#define FALSE (0)
#define TRUE (1)
#define SUCCESS (0)
#define FAIL (1)
#define EMPTY (0)
#define FULL (1)

struct trie 
{
	node_t *node;
	size_t level;
};

struct node 
{
	node_t *children[2];
	int is_subtree_full;
};

static node_t *CreateNodeIMP();
static void RecDestroyIMP(node_t *node);
static int IsSubtreeFullIMP(node_t *parent);
static int InitTrieIMP(trie_t *new_trie, size_t level);
static size_t RecCountAllocIMP(node_t *node, size_t level);
static int IsHaveChildInDirectionIMP(node_t *node, int direction);
static trie_free_status_t RecDeallocateIMP(node_t *current_node, 
										   unsigned int ip, 
										   size_t level);
static trie_alloc_status_t RecInsertIMP(node_t *current_node, 
										size_t level, 
										unsigned int requested_ip);

trie_t *TrieCreate(size_t level)
{
	int init_status = SUCCESS;
	trie_t *new_trie = (trie_t *)malloc(sizeof(trie_t));
	if (NULL == new_trie)
	{
		return NULL;
	}	
	
	init_status = InitTrieIMP(new_trie, level);
	if (FAIL == init_status)
	{
		free(new_trie);
		
		return NULL;
	}

	return new_trie;
}

static int InitTrieIMP(trie_t *new_trie, size_t level)
{
	new_trie->level = level;
	new_trie->node = CreateNodeIMP();
	if (NULL == new_trie->node)
	{
		return FAIL;
	}
	
	return SUCCESS;
}

static node_t *CreateNodeIMP()
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->children[ZERO] = NULL;
	new_node->children[ONE] = NULL;
	new_node->is_subtree_full = EMPTY;
	
	return new_node;
}

void TrieDestroy(trie_t *trie)
{
	assert(trie);

	RecDestroyIMP(trie->node);
		
	free(trie);
}

static void RecDestroyIMP(node_t *node)
{
	if (NULL == node)
	{
		return;
	}
	
	RecDestroyIMP(node->children[ZERO]);
	RecDestroyIMP(node->children[ONE]);
	
	free(node);
}

trie_alloc_status_t TrieInsert(trie_t *trie, unsigned int requested_ip)
{	
	assert(trie);
	
	return RecInsertIMP(trie->node, trie->level, requested_ip);
}

static trie_alloc_status_t RecInsertIMP(node_t *current_node, 
										size_t level, 
										unsigned int requested_ip)
{
	trie_alloc_status_t status = T_SUCCESS_ALLOCATED_REQUESTED;
	unsigned int lsb = 1;
	unsigned int direction = 0;	
	
	if (0 == level)
	{	
		if (EMPTY == current_node->is_subtree_full)
		{
			current_node->is_subtree_full = FULL;
		
			return T_SUCCESS_ALLOCATED_REQUESTED;
		}
		else
		{
			return T_REQUESTED_IP_OCCUPIED;
		}
	}
	
	lsb = 1;
	lsb <<= level - 1;
	direction = requested_ip & lsb;
	direction >>= level - 1;
	
	if (IsHaveChildInDirectionIMP(current_node, direction))
	{
		if (FULL == current_node->is_subtree_full)
		{
			return T_REQUESTED_IP_OCCUPIED;
		}
		else
		{
			status = RecInsertIMP(current_node->children[direction], 
								  level - 1, 
								  requested_ip);
		}
	}
	else
	{
		node_t *new_node = CreateNodeIMP();
		if (NULL == current_node)
		{
			return T_MALLOC_FAIL;
		}
		current_node->children[direction] = new_node;
		status = RecInsertIMP(current_node->children[direction], 
							  level - 1, 
							  requested_ip);
	}
	
	if (IsHaveChildInDirectionIMP(current_node, ZERO) && 
		IsHaveChildInDirectionIMP(current_node, ONE) &&
		T_SUCCESS_ALLOCATED_REQUESTED == status)
	{
		if (IsSubtreeFullIMP(current_node))
		{
			current_node->is_subtree_full = FULL;
		}
	}
	
	return status;
}

trie_free_status_t TrieDeallocate(trie_t *trie, unsigned int ip)
{
	assert(trie);
	
	return RecDeallocateIMP(trie->node, ip, trie->level);
}

static trie_free_status_t RecDeallocateIMP(node_t *current_node, 
										   unsigned int ip, 
										   size_t level)
{
	trie_free_status_t status = T_SUCCESS;
	unsigned int lsb = 1;
	unsigned int direction = 0;
	
	if (NULL == current_node)
	{
		return T_IP_NOT_FOUND;
	}
	
	lsb = 1;
	lsb <<= level - 1;
	direction = ip & lsb;
	direction >>= level - 1;
	
	if (0 == level)
	{	
		if (current_node->is_subtree_full == FULL)
		{
			current_node->is_subtree_full = EMPTY;
			
			return T_SUCCESS;
		}
		else if (current_node->is_subtree_full == EMPTY)
		{
			return T_DOUBLE_FREE;
		}
		else
		{
			return T_IP_NOT_FOUND;
		}
	}
	
	status = RecDeallocateIMP(current_node->children[direction], ip, level - 1);
	
	if (T_SUCCESS == status)
	{
		current_node->is_subtree_full = EMPTY;
	}
	
	return status;
}

size_t TrieCountAlloc(trie_t *trie)
{
	return RecCountAllocIMP(trie->node, trie->level);
}

static size_t RecCountAllocIMP(node_t *node, size_t level)
{
	size_t counter = 0;
	
	if (NULL == node)
	{
		return 0;
	}
	if ((0 == level) && (node->is_subtree_full == FULL))
	{
		return 1;
	}
	counter += RecCountAllocIMP(node->children[ZERO], level - 1);
	counter += RecCountAllocIMP(node->children[ONE], level - 1);
	
	return counter;
}

size_t TrieCountFree(trie_t *trie)
{
	return (pow(2, trie->level) - TrieCountAlloc(trie));
}

int TrieIsFull(trie_t *trie)
{	
	return IsSubtreeFullIMP(trie->node);
}

static int IsHaveChildInDirectionIMP(node_t *node, int direction)
{
	if (NULL != node->children[direction])
	{
		return TRUE;
	}
	
	return FALSE;
}

static int IsSubtreeFullIMP(node_t *parent)
{
	return ((parent->children[ZERO]->is_subtree_full == FULL) && 
			(parent->children[ONE]->is_subtree_full == FULL));
}


