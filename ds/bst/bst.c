 /********************************************
*	Author : Maoz Roytman			
*	Reviewer : 	Gal Salemon
*					bst	
*	Date: Thu Dec 12 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/

#include "bst.h"

#define LEFT -1
#define RIGHT 1
#define FALSE 0
#define TRUE 1
#define OTHER_DIRECTION -1

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

enum kids_state {NO_CHILD = 0,
				LEFT_CHILD = -1, 
				RIGHT_CHILD = 1, 
				TWO_CHILDS = 3};

enum direction {up = 2,
				left = -1, 
				right = 1};
				
typedef enum direction pick_direction;

static bst_node_t *CreateNodeIMP(bst_iter_t parent, void *data);

static int SizeCounterIMP(void *data, void* counter);

static bst_iter_t GetMostDirectionIMP(bst_iter_t node, pick_direction direction);
static bst_node_t *GetDirectionIMP(bst_iter_t node, pick_direction direction);
static bst_iter_t GetRootIMP(bst_t *tree);

static void NullifyChildIMP(bst_iter_t iter, pick_direction direction);

static int WhichChildIAmIMP(bst_iter_t iter, pick_direction direction);
static enum kids_state HowManyKidsIMP(bst_iter_t node);

static bst_iter_t BSTTraverseIMP(bst_iter_t iter, int sign);

static bst_iter_t DestroyAndGoUp(bst_iter_t node);
static void ConnectLegIMP(bst_iter_t parent, bst_iter_t node, 
						  pick_direction direction);

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
	bst_iter_t runner = NULL;
	
	assert(tree);
	
	runner = GetRootIMP(tree);
	
	if (NULL == runner)
	{
		free(tree);
		return;
	}
	
	while (runner != &tree->stub)
	{
		switch (HowManyKidsIMP(runner))
		{
			case NO_CHILD:
				runner = DestroyAndGoUp(runner);
				break;
				
			case LEFT_CHILD:
				runner = GetDirectionIMP(runner, left);
				break;
				
			case RIGHT_CHILD:
				runner = GetDirectionIMP(runner, right);
				break;
				
			case TWO_CHILDS:
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
	int which_child = LEFT;
	
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
	int is_RIGHT_CHILD = WhichChildIAmIMP(iter, right);
	bst_iter_t leftest = {0};
	
	switch (HowManyKidsIMP(iter))
	{
	case NO_CHILD:
		NullifyChildIMP(GetDirectionIMP(iter, up), is_RIGHT_CHILD);
		break;
		
	case LEFT_CHILD:
		ConnectLegIMP(iter->parent, iter->left, is_RIGHT_CHILD);
		break;
		
	case RIGHT_CHILD:
		ConnectLegIMP(iter->parent, iter->right, is_RIGHT_CHILD);
		break;
		
	case TWO_CHILDS:
		leftest = GetMostDirectionIMP(iter->right, left);
		ConnectLegIMP(iter->parent, iter->right, is_RIGHT_CHILD);
		ConnectLegIMP(leftest, iter->left, left);
		break;
		
	}
	free(iter);
}

bst_iter_t BSTFind(bst_t *tree, void *data)
{
	bst_iter_t node = NULL;
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

	return (GetRootIMP((bst_t *)tree) == NULL);
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
	
	BSTForEach(BSTBegin(tree), BSTEnd(tree), SizeCounterIMP, &counter);
	
	return counter;
}

int BSTForEach(bst_iter_t begin, bst_iter_t end, action_func func, void *param)
{
	bst_iter_t node = NULL;
	int func_result = FALSE;	
	
	assert(func);
	
	if (begin == NULL)
	{
		return func_result;
	}
	
	for (node = begin; node != end; node = BSTNext(node))
		{
			func_result = func(BSTGetData(node), param);
			if (func_result != FALSE)
			{
				break;
			}
		}
	return func_result;
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

static int WhichChildIAmIMP(bst_iter_t node, enum direction direction)
{	
	if (BSTIsSameIterator(node, 
						 GetDirectionIMP(GetDirectionIMP(node, up), direction)))
	{
		return TRUE;
	}
	
	return OTHER_DIRECTION;
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

static enum kids_state HowManyKidsIMP(bst_iter_t node)
{
	if (NULL != GetDirectionIMP(node, left) &&
		NULL != GetDirectionIMP(node, right))
	{
		return TWO_CHILDS;
	}
	if (NULL != GetDirectionIMP(node, left))
	{
		return LEFT_CHILD;
	}
	if (NULL != GetDirectionIMP(node, right))
	{
		return RIGHT_CHILD;
	}
	
	return NO_CHILD;
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
	
	if (WhichChildIAmIMP(node, right) == RIGHT)
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
		while (WhichChildIAmIMP(iter, sign) == RIGHT)
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
