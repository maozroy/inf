 /********************************************
*	Author : 			
*	Reviewer : 	
*	bst					
*	Date: Thu Dec 12 17:07:03 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/

#include "bst.h"
#define LEFT -1
#define RIGHT 1

/*
typedef struct bst_node bst_node_t;
typedef bst_node_t *bst_iter_t;
typedef struct bst bst_t;
typedef int (*comparison_func)(const void *new_data, 
							   const void *src_data, 
							   const void *comp_param);
typedef int (*action_func)(void *data, void *for_each_param);
*/

struct bst_node
{
	struct bst_node *parent;
	struct bst_node *left;
	struct bst_node *right;
	void *data;
};

struct bst
{
	comparison_func comparison_func;
	void *comparison_param;
	struct bst_node stub;
};

enum kids_state {no_child = 0,
				left_child = -1, 
				right_child = 1, 
				two_childs = 3};

enum direction {up = 2,
				left = -1, 
				right = 1};
				

static bst_node_t *CreateNodeIMP(bst_node_t *parent, void *data);
static enum kids_state GetNumOfKidsIMP(bst_iter_t node);
static int SizeCounterIMP(void *data, void* counter);

static bst_iter_t GetMostDirectionIMP(bst_iter_t node, enum direction direction);
static bst_node_t *GetDirectionIMP(bst_node_t *node, enum direction direction);
static bst_iter_t GetRootIMP(bst_t *tree);
static void NullifyChildIMP(bst_iter_t iter, enum direction direction);
static int IsASignKidIMP(bst_iter_t node, enum direction direction);

static bst_iter_t BSTTraverseIMP(bst_iter_t iter, int sign);

static bst_iter_t DestroyAndGoUp(bst_iter_t node);

static void ConnectLegIMP(bst_iter_t parent, bst_iter_t node, enum direction d);

bst_t *BSTCreate(comparison_func func, void *param)
{
	bst_t *new_tree = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == new_tree)
	{
		return NULL;
	}

	new_tree->stub.data = (void *)0xDEADBEEF;
	new_tree->stub.right = NULL;
	new_tree->stub.left = NULL;
	new_tree->comparison_func = func;
	new_tree->comparison_param = param;
	
	return new_tree;
}

void BSTDestroy(bst_t *tree)
{
	bst_iter_t runner = GetRootIMP(tree);
	
	assert(tree);
	
	if (NULL == runner)
	{
		free(tree);
		return;
	}
	while (runner != &tree->stub)
	{
		switch (GetNumOfKidsIMP(runner))
		{
			case no_child:
				runner = DestroyAndGoUp(runner);
				break;
				
			case left_child:
				runner = GetDirectionIMP(runner, left);
				break;
				
			case right_child:
				runner = GetDirectionIMP(runner, right);
				break;
				
			case two_childs:
				runner = GetDirectionIMP(runner, right);
				break;
		}
	}
	free(tree);
}

bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	bst_node_t *parent = &tree->stub;
	bst_node_t *runner = GetRootIMP(tree);
	bst_node_t **address_of_parent = &(tree->stub.left);
	int which_child = -1;
	
	assert(tree);
	assert(data);
	
	while (NULL != runner)
	{
		parent = runner;
		which_child = tree->comparison_func(data, runner->data, 
											tree->comparison_param);
		runner = GetDirectionIMP(runner, which_child);
	}
	
	if (which_child == LEFT)
	{
		address_of_parent = &parent->left;
	}
	else
	{
		address_of_parent = &parent->right;
	}
	*address_of_parent = CreateNodeIMP(parent, data);
	
	return *address_of_parent;
}

void BSTRemove(bst_iter_t iter)
{
	int is_right_child = IsASignKidIMP(iter, right);
	bst_iter_t leftest = {0};
	
	switch (GetNumOfKidsIMP(iter))
	{
	case no_child:
		NullifyChildIMP(GetDirectionIMP(iter, up), is_right_child);
		break;
		
	case left_child:
		ConnectLegIMP(iter->parent, iter->left, is_right_child);
		break;
		
	case right_child:
		ConnectLegIMP(iter->parent, iter->right, is_right_child);
		break;
		
	case two_childs:
		leftest = GetMostDirectionIMP(iter->right, left);
		ConnectLegIMP(iter->parent, iter->right, is_right_child);
		ConnectLegIMP(leftest, iter->left, left);
		break;
		
	}
	free(iter);
}

bst_iter_t BSTFind(bst_t *tree, void *data)
{
	bst_iter_t node = {0};
	int which_child = 0;
	
	assert(tree);
	assert(data);
	
	node = GetRootIMP(tree);
	while (NULL != node)
	{
		which_child = tree->comparison_func(data, node->data, 
											tree->comparison_param);
		if (which_child == 0)
		{
			return node;
		}
		node = GetDirectionIMP(node, which_child);
	}
	
	return BSTEnd(tree);
}

int BSTIsEmpty(const bst_t *tree)
{
	assert(tree);

	if ((size_t)&GetRootIMP((bst_t *) tree)->right >= 100)
	{
		return 0;
	}
	if ((size_t)&GetRootIMP((bst_t *) tree)->left >= 100)
	{
		return 0;
	}
	
	return 1;
}

static int SizeCounterIMP(void *data, void* counter)
{
	assert(counter);
	assert(data);

	(void)data;
	*((size_t *)counter) += 1;
	
	return 0;
}

size_t BSTSize(const bst_t *tree)
{
	size_t counter = 0;
	
	BSTForEach(BSTBegin(tree), BSTEnd(tree) ,SizeCounterIMP ,&counter);
	
	return counter;
}

int BSTForEach(bst_iter_t begin, bst_iter_t end, action_func func, void *param)
{
	bst_iter_t node = {0};
	int status = 0;	
	
	assert(func);
	
	if (begin == NULL)
	{
		return status;
	}
	
	for (node = begin ; node != end ; node = BSTNext(node))
		{
			status = func(BSTGetData(node), param);
			if (status != 0)
			{
				break;
			}
		}
	return status;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
	return BSTTraverseIMP(iter, RIGHT);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	return BSTTraverseIMP(iter, LEFT);
}

bst_iter_t BSTBegin(const bst_t *tree)
{
	assert(tree);

	return GetMostDirectionIMP(GetRootIMP((bst_t *) tree), left);
}

bst_iter_t BSTEnd(const bst_t *tree)
{
	assert(tree);

	return (bst_iter_t)&tree->stub;
}

void *BSTGetData(bst_iter_t iter)
{
	return iter->data;
}

int BSTIsSameIterator(bst_iter_t iter1, bst_iter_t iter2)
{	
	if (iter1 == NULL || iter2 == NULL)
	{
		return 0;
	}
	return (&iter1->data == &iter2->data);
}

static int IsASignKidIMP(bst_iter_t node, enum direction direction)
{	
	if (BSTIsSameIterator(node, GetDirectionIMP(GetDirectionIMP(node, up), direction)))
	{
		return 1;
	}
	return -1;
}

static bst_node_t *CreateNodeIMP(bst_iter_t parent, void *data)
{
	bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
	
	new_node->parent = parent;
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	
	return new_node;
}

static bst_iter_t GetRootIMP(bst_t *tree)
{
	assert(tree);
	
	return tree->stub.left;
}
static void ConnectLegIMP(bst_iter_t parent, bst_iter_t node, 
							enum direction direction)
{
	if (direction == RIGHT)
	{
		parent->right = node;
	}
	else
	{
		parent->left = node;
	}
	node->parent = parent;
}

static enum kids_state GetNumOfKidsIMP(bst_iter_t node)
{
	if (NULL != GetDirectionIMP(node, left) &&
		NULL != GetDirectionIMP(node, right))
	{
		return two_childs;
	}
	if (NULL != GetDirectionIMP(node, left))
	{
		return left_child;
	}
	if (NULL != GetDirectionIMP(node, right))
	{
		return right_child;
	}
	
	return no_child;
}
static bst_iter_t GetMostDirectionIMP(bst_iter_t node, enum direction direction)
{
	if (direction == RIGHT && (NULL != node))
	{
		while (NULL != node->right)
		{
			node = GetDirectionIMP(node, direction);
		}
	}
	else if (direction == LEFT && (NULL != node))
	{
		while (NULL != node->left)
		{
			node = GetDirectionIMP(node, direction);
		}
	}
	return node;
}

static bst_node_t *GetDirectionIMP(bst_iter_t node, enum direction direction)
{
	bst_iter_t returned_iter = NULL;
	
	if (NULL == node)
	{
		return returned_iter;
	}
	
	switch (direction)
	{
		case up:
			returned_iter = node->parent;
			break;
		case right:
			returned_iter = node->right;
			break;
		case left:
			returned_iter = node->left;
			break;
	}
	return returned_iter;
}

static bst_iter_t DestroyAndGoUp(bst_iter_t node)
{
	bst_iter_t parent = NULL;
	parent = GetDirectionIMP(node, up);
	
	if (IsASignKidIMP(node, right) == RIGHT)
	{
		parent->right = NULL;
	}
	else
	{
		parent->left = NULL;
	}
	free(node);
	
	return parent;
}

static bst_iter_t BSTTraverseIMP(bst_iter_t iter, int sign)
{
	bst_iter_t child = NULL;

		child = GetDirectionIMP(iter, sign);
		
		if (child != NULL)
		{
			child = GetMostDirectionIMP(child, sign * LEFT);
			iter = child;
		}
		else
		{
			while (IsASignKidIMP(iter, sign) == RIGHT)
			{
				iter = GetDirectionIMP(iter, up);
			}
			iter = GetDirectionIMP(iter, up);
		}
	return iter;
}

static void NullifyChildIMP(bst_iter_t iter, enum direction direction)
{
	if (direction == left)
	{
		iter->left = NULL;
	}
	else
	{
		iter->right = NULL;
	}
}
