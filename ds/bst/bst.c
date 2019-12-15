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

bst_node_t *CreateNodeIMP(bst_node_t *parent, void *data);
bst_node_t *GetRightIMP(bst_node_t *node);
bst_node_t *GetLeftIMP(bst_node_t *node);
bst_node_t *GetParentIMP(bst_node_t *node);
bst_iter_t GetRootIMP(bst_t *tree);

bst_t *BSTCreate(comparison_func func, void *param)
{
	bst_t *new_tree = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == new_tree)
	{
		return NULL;
	}

	new_tree->stub.data = (void *)0xDEADBEEF;
	new_tree->comparison_func = func;
	new_tree->comparison_param = param;
	
	return new_tree;
}
/*
void BSTDestroy(bst_t *tree)
{
	bst_iter_t runner = tree->stub.left;
	while (runner != NULL)
	{
		
	
	}

}
*/

void *BSTGetData(bst_iter_t iter)
{
	return iter->data;
}

int BSTIsSameIterator(bst_iter_t iter1, bst_iter_t iter2)
{
	return (&iter1->data == &iter2->data);
}

bst_iter_t BSTBegin(const bst_t *tree)
{
	bst_iter_t node = GetRootIMP((bst_t *) tree);
	while (node->left != NULL)
	{
		node = GetLeftIMP(node);
	}
	
	return node;
}
bst_iter_t BSTEnd(const bst_t *tree)
{
	bst_iter_t node = GetRootIMP((bst_t *) tree);
	while (node->right != NULL)
	{
		node = GetRightIMP(node);
	}
	return node;
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

bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	bst_node_t *parent = &tree->stub;
	bst_node_t *runner = GetRootIMP(tree);
	bst_node_t **new_node_parent_location = &(tree->stub.left);
	int which_child = -1;
	
	while(NULL != runner)
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

bst_node_t *CreateNodeIMP(bst_node_t *parent, void *data)
{
	bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
	
	new_node->parent = parent;
	new_node->data = data;
	
	return new_node;
}

bst_node_t *GetRightIMP(bst_node_t *node)
{
	return node->right;
}

bst_node_t *GetLeftIMP(bst_node_t *node)
{
	return node->left;
}

bst_node_t *GetParentIMP(bst_node_t *node)
{
	return node->parent;
}

bst_iter_t GetRootIMP(bst_t *tree)
{
	return tree->stub.left;
}




