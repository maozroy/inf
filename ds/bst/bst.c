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
				left_child = 1, 
				right_child = 2, 
				two_childs = 3};

enum direction {up = 2,
				left = -1, 
				right = 1};
				

static bst_node_t *CreateNodeIMP(bst_node_t *parent, void *data);

static bst_node_t *GetDirectionIMP(bst_node_t *node, enum direction direction);
static bst_iter_t GetRootIMP(bst_t *tree);

static int IsARightKidIMP(bst_iter_t node);

static bst_iter_t BSTTraverseIMP(bst_iter_t iter, int sign);

static enum kids_state GetNumOfKidsIMP(bst_iter_t node);
static bst_iter_t GetMostLeftIMP(bst_iter_t node);
static bst_iter_t DestroyAndGoUp(bst_iter_t node);

static void ConnectLeftLegIMP(bst_iter_t parent, bst_iter_t node);
static void ConnectRightLegIMP(bst_iter_t parent, bst_iter_t node);

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
	if (runner == NULL)
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


bst_iter_t BSTNext(bst_iter_t iter)
{
	return BSTTraverseIMP(iter, 1);
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	return BSTTraverseIMP(iter, -1);
}


static bst_iter_t BSTTraverseIMP(bst_iter_t iter, int sign)
{
	bst_iter_t child = NULL;
	
	if (sign == 1)
	{	
		child = GetDirectionIMP(iter, right);
		if (child != NULL)
		{
			
			while (child->left != NULL)
			{
				child = GetDirectionIMP(child, left);
			}
			iter = child;
		}
		else
		{
			while (IsARightKidIMP(iter))
			{
				iter = GetDirectionIMP(iter, up);
			}
			iter = GetDirectionIMP(iter, up);
		}
	}
	else
	{
		child = GetDirectionIMP(iter, left);
		
		if (child != NULL)
		{
			
			while (child->right != NULL)
			{
				child = GetDirectionIMP(child, right);
			}
			iter = child;
		}
		else
		{
			while (!IsARightKidIMP(iter))
			{
				iter = GetDirectionIMP(iter, up);
			}
			iter = GetDirectionIMP(iter, up);
		}	
	}
	return iter;
}

void BSTRemove(bst_iter_t iter)
{
	int is_right_child = IsARightKidIMP(iter);
	bst_iter_t leftest = {0};
	
	switch (GetNumOfKidsIMP(iter))
	{
	case no_child:
		if (is_right_child)
		{
			iter->parent->right = NULL;
		}
		else
		{
			iter->parent->left = NULL;
		}
		break;
		
	case left_child:
		if (is_right_child)
		{
			ConnectRightLegIMP(iter->parent, iter->left);
		}
		else
		{
			ConnectLeftLegIMP(iter->parent, iter->left);
		}
		break;
		
	case right_child:
		if (is_right_child)
		{
			ConnectRightLegIMP(iter->parent, iter->right);
		}
		else
		{
			ConnectLeftLegIMP(iter->parent, iter->right);
		}
		break;
		
	case two_childs:
		leftest = GetMostLeftIMP(iter->right);
		if (is_right_child)
		{
			ConnectRightLegIMP(iter->parent, iter->right);
		}
		else
		{
			ConnectLeftLegIMP(iter->parent, iter->right);
		}
		ConnectLeftLegIMP(leftest, iter->left);
	}
	free(iter);
}

size_t BSTSize(const bst_t *tree)
{
	bst_iter_t node = {0};
	size_t counter = 0;
	
	if(BSTIsEmpty(tree) == 1)
	{
		return 0;
	}
	
	for (node = BSTBegin(tree) ;
		node != BSTEnd(tree) ;
		node = BSTNext(node))
	{
		++counter;
	}
	return counter;
}

static int IsARightKidIMP(bst_iter_t node)
{	
	if (BSTIsSameIterator(node, GetDirectionIMP(node, up)->right))
	{
		return 1;
	}
	return 0;
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


bst_iter_t BSTBegin(const bst_t *tree)
{
	bst_iter_t node = GetRootIMP((bst_t *) tree);
	
	node = GetMostLeftIMP(node);
	
	return node;
}

bst_iter_t BSTEnd(const bst_t *tree)
{
	return (bst_iter_t)&tree->stub;
}

int BSTIsEmpty(const bst_t *tree)
{
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

bst_iter_t BSTFind(bst_t *tree, void *data)
{
	bst_iter_t node = {0};
	int which_child = 0;
	node = GetRootIMP(tree);
	
	while (NULL != node)
	{
		which_child = tree->comparison_func
							(data, node->data, 
							tree->comparison_param);
		if (which_child == 0)
		{
			return node;
		}
		node = GetDirectionIMP(node, which_child);
	}
	return BSTEnd(tree);
}

int BSTForEach(bst_iter_t begin, bst_iter_t end, action_func func, void *param)
{
	bst_iter_t node = {0};
	int status = 0;	
	for (node = begin ;
		node != end ;
		node = BSTNext(node))
		{
			status = func(BSTGetData(node), param);
			if (status != 0)
			{
				break;
			}
		}
	return status;
}


bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	bst_node_t *parent = &tree->stub;
	bst_node_t *runner = GetRootIMP(tree);
	bst_node_t **new_node_parent_location = &(tree->stub.left);
	int which_child = -1;
	
	while (NULL != runner)
	{
		parent = runner;
		which_child = tree->comparison_func
							(data, runner->data, 
							tree->comparison_param);
		if (which_child < 0)
		{
			new_node_parent_location = &parent->left;
			runner = runner->left;
		}
		else
		{
			new_node_parent_location = &parent->right;
			runner = runner->right;
		}
	}
	*new_node_parent_location = CreateNodeIMP(parent, data);
	
	return *new_node_parent_location;
}

static bst_node_t *CreateNodeIMP(bst_node_t *parent, void *data)
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
	return tree->stub.left;
}

static void ConnectLeftLegIMP(bst_iter_t parent, bst_iter_t node)
{
	parent->left = node;
	node->parent = parent;
}

static void ConnectRightLegIMP(bst_iter_t parent, bst_iter_t node)
{
	parent->right = node;
	node->parent = parent;
}

static enum kids_state GetNumOfKidsIMP(bst_iter_t node)
{
	if (GetDirectionIMP(node, left) != NULL && GetDirectionIMP(node, right) != NULL)
	{
		return two_childs;
	}
	if (GetDirectionIMP(node, left) != NULL)
	{
		return left_child;
	}
	if (GetDirectionIMP(node, right) != NULL)
	{
		return right_child;
	}
	
	return no_child;
}


static bst_iter_t GetMostLeftIMP(bst_iter_t node)
{
	while (node->left != NULL)
	{
		node = GetDirectionIMP(node, left);
	}
	return node;	
}


static bst_node_t *GetDirectionIMP(bst_node_t *node, enum direction direction)
{
	bst_iter_t returned_iter = NULL;
	
	if (node == NULL)
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
	
	if (IsARightKidIMP(node))
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

