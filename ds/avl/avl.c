 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	
*	avl					
*	Date: Mon Dec 23 17:52:33 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/

#include "avl.h"

typedef struct avl_node avl_node_t;
struct avl_node
{
	avl_node_t *children[2];
	void *data;
	size_t height; 
};

struct avl
{
	comparison_func_t comparison_func;
	avl_node_t *root;
};

enum direction
{
	NONE = -1,
	LEFT = 0,
	RIGHT = 1,
	BOTH = 2
};

static avl_node_t *RecInsertIMP(avl_node_t **node, comparison_func_t func, void *data);
static avl_node_t **GetRootAdrsIMP(avl_t *tree);
static int GetDirectionIMP(int result);
static size_t UpdateHeightIMP(avl_node_t *node);
static void RecDestroyIMP(avl_node_t *node);
static avl_node_t *CreateNodeIMP(void *data);
static void RecSizeIMP(avl_node_t *node, size_t *size);
static size_t MaxIMP(size_t a, size_t b);
static int GetNumOfKidsIMP(avl_node_t *node);
static avl_node_t *RecFindIMP(avl_node_t *node, comparison_func_t func, void *data);
static int RecForEachIMP(avl_node_t *node, action_func_t func, void* param);
static void RecRemoveIMP(avl_node_t *node, void *data, avl_t *tree);
static void ConnectToLeftChildIMP(avl_node_t *node, avl_node_t *node_to_connect);
static enum direction IsMyChildTheChosenOneIMP
		(avl_node_t *node, void *data, comparison_func_t func);

static void RemoveByParentIMP(avl_node_t *node_to_remove,enum direction direction);
static void RemoveRootIMP(avl_node_t *node, avl_t *tree);

avl_t *AVLCreate(comparison_func_t func)
{
	avl_t *new_tree = (avl_t *)malloc(sizeof(avl_t));
	if (NULL == new_tree)
	{
		return NULL;
	}
	
	new_tree->root = NULL;
	new_tree->comparison_func = func;

	return new_tree;
}

int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t **root = GetRootAdrsIMP(tree);
	avl_node_t *new_node = RecInsertIMP(root, tree->comparison_func, data);
	if (NULL == new_node)
	{
		return 1;
	}
	return 0;
}

static int GetDirectionIMP(int result)
{
	if (result == -1)
	{
		return LEFT;
	}

	return result;
}

static avl_node_t **GetRootAdrsIMP(avl_t *tree)
{
	return &(tree->root);
}

static size_t UpdateHeightIMP(avl_node_t *node)
{
	int num_of_kids = 0;
	int result = 0;
	
	if (NULL == node)
	{
		return 0;
	}
	
	num_of_kids = GetNumOfKidsIMP(node);
	
	switch (num_of_kids)
	{
		case NONE:
			result = 0;
			break;
		case RIGHT:
			result = node->children[RIGHT]->height + 1;
			break;
		case LEFT:
			result = node->children[LEFT]->height + 1;
			break;
		case BOTH:
			result = MaxIMP(node->children[LEFT]->height, 
					node->children[RIGHT]->height) + 1;
			break;
	}
	return result;
}

static int GetNumOfKidsIMP(avl_node_t *node)
{
	if (NULL == node)
	{
		return NONE;
	}

	if ((NULL == node->children[LEFT]) && (NULL == node->children[RIGHT]))
	{
		return NONE;
	}
	else if (NULL != node->children[LEFT] && NULL != node->children[RIGHT])
	{
		return BOTH;
	}
	else if (NULL == node->children[LEFT])
	{
		return RIGHT;
	}
	else
	{
		return LEFT;
	}
}

size_t AVLSize(const avl_t *tree)
{
	size_t size = 0;
	avl_node_t *root = tree->root;
	if (NULL == root)
	{
		return 0;
	}
	RecSizeIMP(root, &size);

	return size;
}
static void RecSizeIMP(avl_node_t *node, size_t *size)
{	
	(*size) += 1;
	
	switch (GetNumOfKidsIMP(node))
	{
		case LEFT:
			RecSizeIMP(node->children[LEFT], size);
			break;
		case RIGHT:
			RecSizeIMP(node->children[RIGHT], size);
			break;
		case BOTH:
			RecSizeIMP(node->children[LEFT], size);
			RecSizeIMP(node->children[RIGHT], size);
			break;
	}
}

void *AVLFind(const avl_t *tree, const void *data)
{
	avl_node_t *node = RecFindIMP(tree->root, tree->comparison_func, (void *)data);
	
	if (NULL == node)
	{
		return NULL;
	}

	return node->data;

}
static avl_node_t *RecFindIMP(avl_node_t *node, comparison_func_t func, void *data)
{
	int cmp_result = 0;
	
	if (node == NULL)
	{
		return NULL;
	}
	
	cmp_result = func(data, node->data);
	if (cmp_result == 0)
	{
		return node;
	}
	if (node->children[GetDirectionIMP(cmp_result)] == NULL)
	{
		return NULL;
	}
	return RecFindIMP(node->children[GetDirectionIMP(cmp_result)], func, data);
}

int AVLForEach(avl_t *tree, action_func_t func, void *param)
{
	return RecForEachIMP(tree->root, func, param);
}
static enum direction IsMyChildTheChosenOneIMP(avl_node_t *node, void *data,
												comparison_func_t func)
{
	int parent_direciton = GetNumOfKidsIMP(node);
	int cmp_result_right = 1;
	int cmp_result_left = 1;
	switch (parent_direciton)
	{
		case RIGHT:
			cmp_result_right = func(data, node->children[RIGHT]->data);
			break;
		case LEFT:
			cmp_result_left = func(data, node->children[LEFT]->data);
			break;
		case BOTH:
			cmp_result_left = func(data, node->children[LEFT]->data);
			cmp_result_right = func(data, node->children[RIGHT]->data);
			break;
	}
	if (cmp_result_right == 0)
	{
		return RIGHT;
	}
	if (cmp_result_left == 0)
	{
		return LEFT;
	}
	return NONE;
}


static void RecRemoveIMP(avl_node_t *node, void *data, avl_t *tree)
{
	int cmp_result = 0;
	int direction = NONE;
	comparison_func_t func = tree->comparison_func;

	if (node == NULL)
	{
		return;
	}
	
	cmp_result = func(data, node->data);
	if (cmp_result == 0)
	{
		RemoveRootIMP(node, tree);
		tree->root->height = UpdateHeightIMP(tree->root);
		return;
	}
	
	direction = IsMyChildTheChosenOneIMP(node, data, func);
	if (direction != NONE)
	{
		RemoveByParentIMP(node, direction);
		node->height = UpdateHeightIMP(node);
		return;
	}
	

	RecRemoveIMP(node->children[GetDirectionIMP(cmp_result)],data, tree);
	node->children[GetDirectionIMP(cmp_result)]->height = 
	UpdateHeightIMP(node->children[GetDirectionIMP(cmp_result)]);
	return;
}

static void RemoveRootIMP(avl_node_t *node, avl_t *tree)
{
	switch (GetNumOfKidsIMP(node))
	{
		case NONE:
			tree->root = NULL;
			break;
			
		case RIGHT:
			tree->root = node->children[RIGHT];
			break;
			
		case LEFT:
			tree->root = node->children[LEFT];
			break;
			
		case BOTH:
			ConnectToLeftChildIMP(node->children[RIGHT], node->children[LEFT]);
			tree->root = node->children[RIGHT];
			break;
	}
	free(node);
}

static void RemoveByParentIMP(avl_node_t *node, enum direction direction)
{
	avl_node_t *node_to_remove = node->children[direction];
	
	switch (GetNumOfKidsIMP(node_to_remove))
	{
		case NONE:
			node->children[direction] = NULL;
			break;
		case RIGHT:
			node->children[direction] = node_to_remove->children[RIGHT];
			break;
		case LEFT:
			node->children[direction] = node_to_remove->children[LEFT];
			break;
		case BOTH:
			ConnectToLeftChildIMP(node_to_remove->children[RIGHT], node_to_remove->children[LEFT]);
			node->children[direction] = node_to_remove->children[RIGHT];
			break;
	}
	free(node_to_remove);
}

static void ConnectToLeftChildIMP(avl_node_t *node, avl_node_t *node_to_connect)
{
	if (NULL == node->children[LEFT])
	{
		node->children[LEFT] = node_to_connect;
		node->height = UpdateHeightIMP(node);
		
		return;
	}
	ConnectToLeftChildIMP(node->children[LEFT], node_to_connect);
	node->height = UpdateHeightIMP(node);
	
	return;
}

void AVLRemove(avl_t *tree, const void *data)
{
	if (data == NULL)
	{
		return;
	}
	RecRemoveIMP(tree->root, (void *)data, tree);

}

static int RecForEachIMP(avl_node_t *node, action_func_t func, void* param)
{
	int func_res = 0;
	
	if (NULL == node)
	{
		return 0;
	}

	RecForEachIMP(node->children[LEFT], func, param);
	if (NULL == node)
	{
		return 0;
	}
	func_res = func(node->data, param);
	if (0 != func_res)
	{
		return func_res;
	}

	return RecForEachIMP(node->children[RIGHT], func, param);
}


static avl_node_t *RecInsertIMP(avl_node_t **node, comparison_func_t func, void *data)
{
	int cmp_result = 0;
	int direction = 0;
	
	if (NULL == *node)
	{
		*node = CreateNodeIMP(data);
		if (NULL == node)
		{
			return NULL;
		}
	}
	else
	{
		cmp_result = func(data, (*node)->data);
		direction = GetDirectionIMP(cmp_result);
		(*node)->children[direction] = RecInsertIMP(&((*node)->children[direction]), func, data);
	}
	 (*node)->height = UpdateHeightIMP(*node);
	/*BalanceIMP*/
	return *node;
}

void AVLDestroy(avl_t *tree)
{
	RecDestroyIMP(*(GetRootAdrsIMP(tree)));
	
	free(tree);
}

static void RecDestroyIMP(avl_node_t *node)
{
	if (node != NULL)
	{
		RecDestroyIMP(node->children[LEFT]);
		RecDestroyIMP(node->children[RIGHT]);
		
		free(node);
		node = NULL;
	}
}

size_t AVLGetHeight(const avl_t *tree)
{
	if (NULL == tree->root)
	{
		return 0;
	}

	return tree->root->height;
}

int AVLIsEmpty(const avl_t *tree)
{
	return (tree->root == NULL);
}

static size_t MaxIMP(size_t a, size_t b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}

static avl_node_t *CreateNodeIMP(void *data)
{
	avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	new_node->data = data;
	new_node->height = 0;
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;
	
	return new_node;

}

