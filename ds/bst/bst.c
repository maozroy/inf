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
bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	bst_node_t *parent = &tree->stub;
	bst_node_t *runner = tree->stub.left;
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







