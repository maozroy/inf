 /********************************************
*	Author : Maoz Roytman			
*	Reviewer : 	Sharon Rottner
*	trie					
*	Date: 6/1/2020	                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <math.h> /*pow*/

#include "trie.h"


struct trie 
{
	node_t *node;
	size_t height;
};

struct node 
{
	node_t *children[2];
	int is_subtree_full;
};

enum is_node_free
{
	FREE,
	FULL
};

enum kids
{
	LEFT = 0,
	RIGHT = 1
};

node_t *CreateNodeIMP(void);
static unsigned int GoToNexTIMP(unsigned int ip, size_t height);
static void RecTrieDestroyIMP(node_t *node);
static int IsThereTwoChildrenIMP(node_t *node);
static int IsAnyChildEmptyIMP(node_t *node);
static node_t *RecTrieInsertIMP(node_t *node, 
								size_t height, 
								unsigned int requested_ip, 
								trie_alloc_status_t *status);
static void RecTrieDeallocateIMP (node_t *node, size_t height, 
										   unsigned int ip, 
										   trie_free_status_t *status);
static void RecTrieCountAlloc(node_t *node, size_t height, size_t *counter);

trie_t *TrieCreate(size_t height)
{
	trie_t *tree = NULL;
	node_t *root = NULL;
	
	assert(height);
	
	tree = (trie_t *)malloc(sizeof(trie_t));
	if (NULL == tree)
	{
		return NULL;
	}
	
	root = CreateNodeIMP();
	if (NULL == root)
	{
		free(tree);
		return NULL;
	}
	
	tree -> node = root;
	tree -> height = height;
	
	return tree;
}

node_t *CreateNodeIMP(void)
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	new_node -> children[LEFT] = NULL;
	new_node -> children[RIGHT] = NULL;
	new_node -> is_subtree_full = FREE;
	
	return new_node;
}

void TrieDestroy(trie_t *trie)
{
	assert(trie);

	RecTrieDestroyIMP(trie -> node);
	free(trie);
}

void RecTrieDestroyIMP(node_t *node)
{
	if (node -> children[LEFT] != NULL)
	{
		RecTrieDestroyIMP(node -> children[LEFT]);
	}
	if (node -> children[RIGHT] != NULL)
	{
		RecTrieDestroyIMP(node -> children[RIGHT]);
	}
	
	free(node);
}

static unsigned int GoToNexTIMP(unsigned int ip, size_t height)
{
	unsigned int which_bit = (ip>>(height - 1)) & 1;

	return which_bit;
}

trie_alloc_status_t TrieInsert(trie_t *trie, unsigned int requested_ip)
{
	trie_alloc_status_t status = T_SUCCESS_ALLOCATED_REQUESTED;
	
	assert(trie);
	
	RecTrieInsertIMP(trie -> node, trie -> height, requested_ip, &status);
	
	return status;
}

static node_t *RecTrieInsertIMP(node_t *node, size_t height, 
								unsigned int requested_ip, 
								trie_alloc_status_t *status)
{
	int direction = 0;
	if (T_SUCCESS_ALLOCATED_REQUESTED != *status)
	{
		return node;
	}
	if (NULL == node)
	{
		node = CreateNodeIMP();
		
		if (NULL == node)
		{
			*status = T_MALLOC_FAIL;
			return node;
		}
	}
	
	if (FULL == node -> is_subtree_full)
	{
		*status = T_REQUESTED_IP_OCCUPIED;
		
		return node;
	}
	
	if (0 == height)
	{
		node -> is_subtree_full = FULL;
		status = T_SUCCESS;
		
		return node;
	}
	direction = GoToNexTIMP(requested_ip, height);
	node -> children[direction] = RecTrieInsertIMP(node -> children[direction], 
												   height - 1, 
												   requested_ip, 
												   status);

	if (IsThereTwoChildrenIMP(node) && !IsAnyChildEmptyIMP(node))
	{
		node -> is_subtree_full = FULL;
	}
	
	return node;
}

static int IsThereTwoChildrenIMP(node_t *node)
{
	return ((NULL != node -> children[LEFT]) && 
			(NULL != node -> children[RIGHT]));
}

static int IsAnyChildEmptyIMP(node_t *node)
{
	return ((FREE == node -> children[LEFT] -> is_subtree_full) ||
		   (FREE == node -> children[RIGHT] -> is_subtree_full));
}

trie_free_status_t TrieDeallocate(trie_t *trie, unsigned int ip)
{
	trie_free_status_t status = T_SUCCESS;
	
	assert(trie);
	
	RecTrieDeallocateIMP(trie -> node, trie -> height, ip, &status);
	
	return status;
}

static void RecTrieDeallocateIMP (node_t *node, size_t height, 
										   unsigned int ip, 
										   trie_free_status_t *status)
{
	int direction = 0;
	
	direction = GoToNexTIMP(ip, height);
	if (NULL == node)
	{
		*status = T_IP_NOT_FOUND;
		
		return;
	}
	else if (0 == height)
	{
		if (node -> is_subtree_full == FREE)
		{
			*status = T_DOUBLE_FREE;
			return;
		}
		else
		{
			node -> is_subtree_full = FREE;
			*status = T_SUCCESS;
			return;
		}
	}
	
	RecTrieDeallocateIMP (node -> children[direction], height - 1, ip, status);
	node -> is_subtree_full = FREE; 
}

size_t TrieCountAlloc(trie_t *trie)
{
	size_t counter = 0;
	RecTrieCountAlloc(trie -> node, trie -> height, &counter);
	
	return counter;
}

static void RecTrieCountAlloc(node_t *node, size_t height, size_t *counter)
{
	if (NULL == node)
	{
		return;
	}
	
	if (FULL == node -> is_subtree_full)
	{
		*counter += pow(2, height);
		return;
	}
	
	if(1 == height)
	{
		if ((NULL != node -> children[LEFT]) && 
			(FULL == node ->children[LEFT] -> is_subtree_full))
		{
			*counter += 1;
		}
		
		if ((NULL != node -> children[RIGHT]) && 
			(FULL == node ->children[RIGHT] -> is_subtree_full))
		{
			*counter += 1;
		}
		return;
	}

	RecTrieCountAlloc(node -> children[LEFT], height - 1, counter);
	RecTrieCountAlloc(node -> children[RIGHT], height - 1, counter);
}

size_t TrieCountFree(trie_t *trie)
{
	assert(trie);

	return pow(2,(trie -> height)) - TrieCountAlloc(trie);
}

int TrieIsFull(trie_t *trie)
{
	assert(trie);

	return (trie -> node -> is_subtree_full == FULL);
}
