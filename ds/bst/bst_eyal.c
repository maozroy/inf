 /********************************************
*	Author : Eyal Finkelshtein				
*	Reviewer : Maoz Roytman
*	Binary Search Tree					
*	Date: 16.1.2019		                
*																				
********************************************/

#include <stdlib.h> /* for malloc */
#include <assert.h> /* for assert */

#include "bst.h"
#include "my_utils.h"

#define LEFT 100
#define RIGHT 200
 
struct bst_node
{
	bst_node_t *parent;
	bst_node_t *left;
	bst_node_t *right;
	void *data;
};

struct bst
{
	comparison_func comparison_func_ptr;
	void *comparison_param;
	bst_node_t stub;
};

enum move_direction{GO_UP = 10, GO_RIGHT = 1, GO_LEFT = -1}; 

static bst_iter_t MoveAllTheWayDownFromIterIMP(bst_iter_t iter, int move_direction);
static int Count(void *data, void *counter);
static bst_iter_t MoveToIMP(bst_iter_t iter, int move_direction);
static bst_node_t *BuildNewNodeIMP(void *data);
static void DestroryIterIMP(bst_iter_t *iter);
static int IsRightChildIMP(bst_iter_t iter);
static int IsLeftChildIMP(bst_iter_t iter);
static int HasRightChildIMP(bst_iter_t iter);
static int HasLeftChildIMP(bst_iter_t iter);

static int IsLeafIMP(bst_iter_t iter);
static void HandleIterInDestroyIMP(bst_iter_t *iter, int direction);
static void RemoveLeafIMP(bst_iter_t iter);
static void RemoveNodeWithOneChildIMP(bst_iter_t iter);
static void RemoveNodeWithTwoChildrenIMP(bst_iter_t iter);
static void UpdateParentAndPositionIMP(bst_iter_t **position_for_new_node,
			bst_iter_t *new_node_parent, bst_iter_t *address_of_new_node,
			bst_iter_t parent_node);

static void ConnectChildToParentIMP(bst_iter_t parent, 
									bst_iter_t child, int connect_direction);

static void FindPositionToInsert(bst_iter_t current_node, 
			bst_iter_t *new_node_parent,
			bst_iter_t **position_for_new_node, void *data,bst_t *tree);
									
bst_t *BSTCreate(comparison_func func, void *param)
{
	bst_t *bst = NULL;
	bst_node_t stub = {0};
	
	assert(func);
	
	bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == bst)
	{		
		return NULL;
	}
		
	stub.data = "DEADBEAF";
	
	bst->comparison_func_ptr = func;
	bst->comparison_param = param;
	bst->stub = stub;
	
	return bst;
}

void BSTRemove(bst_iter_t iter)
{	
	assert(iter);
	
	if (IsLeafIMP(iter))
	{
		RemoveLeafIMP(iter);
	}	
	else if(HasRightChildIMP(iter) && (HasLeftChildIMP(iter)))	
	{
		RemoveNodeWithTwoChildrenIMP(iter);
	}	
	else
	{
		RemoveNodeWithOneChildIMP(iter);
	}
	
	free(iter);
}

bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	bst_iter_t *position_for_new_node = NULL;
	bst_iter_t new_node = NULL;
	bst_iter_t current_node = NULL;
	bst_iter_t new_node_parent = NULL;
	
	new_node = BuildNewNodeIMP(data);
	if (NULL == new_node)
	{
		return NULL;
	}
		
	assert(tree);
	
	position_for_new_node = &(tree->stub.left);
	current_node = tree->stub.left;
	new_node_parent = &tree->stub;
	
	FindPositionToInsert(current_node, &new_node_parent,
					&position_for_new_node, data, tree);

	new_node->parent = new_node_parent;	
	*position_for_new_node = new_node;
	
	return new_node;
}

int BSTForEach(bst_iter_t begin, bst_iter_t end, 
			   action_func run_action, void *param)
{
	bst_iter_t iter_runner = begin;
	int func_result = 0;
	
	for (iter_runner = begin; 
		 end != iter_runner; 
		 iter_runner = BSTNext(iter_runner))
	{
		func_result = run_action(BSTGetData(iter_runner), param);
				
		if (0 != func_result)
		{
			break;
		}		
	}
	
	return func_result;
}

size_t BSTSize(const bst_t *tree)
{
	int counter = 0;
	
	assert(tree);
		
	BSTForEach(BSTBegin(tree), BSTEnd(tree), 
			   Count, &counter);
	
	return counter;
}

bst_iter_t BSTFind(bst_t *tree, void *data)
{
	bst_iter_t iter_runner = NULL;
	int comparison_func_result = 0;
	
	assert(tree);
	
	iter_runner = (tree->stub).left;
	
	while (NULL != iter_runner)
	{
		comparison_func_result = tree->comparison_func_ptr(
							data, iter_runner->data, tree->comparison_param);
							
		if (0 == comparison_func_result)
		{
			return iter_runner;
		}
		
		iter_runner = MoveToIMP(iter_runner, comparison_func_result);		   
	}
	
	return BSTEnd(tree);
}

void BSTDestroy(bst_t *tree)
{
	bst_iter_t iter_runner = (bst_iter_t )(&tree->stub);
	
	assert(tree);
		
	while (!(BSTIsEmpty(tree)))
	{
		if (IsLeafIMP(iter_runner))
		{
			iter_runner = MoveToIMP(iter_runner, GO_UP);
		}
		
		else if (HasRightChildIMP(iter_runner))
		{
			HandleIterInDestroyIMP(&iter_runner, RIGHT);
		}
		else if (HasLeftChildIMP(iter_runner))
		{
			HandleIterInDestroyIMP(&iter_runner, LEFT);
		}					
	}
	
	free(tree);
}

bst_iter_t BSTEnd(const bst_t *tree)
{
	assert(tree);
	
	return ((bst_iter_t) &(tree->stub));
}

int BSTIsEmpty(const bst_t *tree)
{
	assert(tree);
	
	return (IsLeafIMP((bst_iter_t )(&tree->stub)));	
}

bst_iter_t BSTBegin(const bst_t *tree)
{
	bst_iter_t stub = {0};
	
	assert(tree);
	
	stub = (bst_iter_t) &(tree->stub);
	
	return MoveAllTheWayDownFromIterIMP(stub, GO_LEFT);
}

void *BSTGetData(bst_iter_t iter)
{
	return iter->data;
}


bst_iter_t BSTPrev(bst_iter_t iter)
{
	if (HasLeftChildIMP(iter))
	{
		iter = MoveToIMP(iter, GO_LEFT);		
		iter = MoveAllTheWayDownFromIterIMP(iter, GO_RIGHT);
		
		return iter;
	}
	else
	{
		while (IsLeftChildIMP(iter))
		{
			iter = MoveToIMP(iter, GO_UP);
		}	
		
		return iter->parent;	
	}
}

bst_iter_t BSTNext(bst_iter_t iter)
{
	if (HasRightChildIMP(iter))
	{
		iter = MoveToIMP(iter, GO_RIGHT);		
		iter = MoveAllTheWayDownFromIterIMP(iter, GO_LEFT);		
		
		return iter;
	}
	else
	{
		while (IsRightChildIMP(iter))
		{
			iter = MoveToIMP(iter, GO_UP);
		}	
		
		return iter->parent;	
	}
}

int BSTIsSameIterator(bst_iter_t iter1, bst_iter_t iter2)
{
	return (iter1 == iter2);
}

static int IsRightChildIMP(bst_iter_t iter)
{
	return (BSTIsSameIterator((iter->parent)->right, iter));
}

static int IsLeftChildIMP(bst_iter_t iter)
{
	return (BSTIsSameIterator((iter->parent)->left, iter));
}

static int HasRightChildIMP(bst_iter_t iter)
{
	return (NULL != iter->right);
}

static int HasLeftChildIMP(bst_iter_t iter)
{
	return (NULL != iter->left);
}

static int IsLeafIMP(bst_iter_t iter)
{
	if (!(HasRightChildIMP(iter)) && (!HasLeftChildIMP(iter)))
	{
		return 1;
	}
	
	return 0;
}

static bst_iter_t MoveToIMP(bst_iter_t iter, int move_direction)
{
	switch(move_direction)
    	{	
		    case GO_UP:
		       return (iter->parent);
		       
		    case GO_LEFT:
		        return (iter->left);
		        
		    case GO_RIGHT:
		       return (iter->right);
		}
		
		return iter;
}

static bst_iter_t MoveAllTheWayDownFromIterIMP(bst_iter_t iter, int move_direction)
{
	if(GO_LEFT == move_direction)
	{
		while (HasLeftChildIMP(iter))
		{
			iter = MoveToIMP(iter, move_direction);
		}
	}
	else if (GO_RIGHT == move_direction)
	{
		while (HasRightChildIMP(iter))
		{
			iter = MoveToIMP(iter, move_direction);
		}
	}    	
	
	return iter;
}

static bst_node_t *BuildNewNodeIMP(void *data)
{
	bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->data = data;
	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	
	return new_node;
}
static void ConnectChildToParentIMP(bst_iter_t parent, bst_iter_t child,
									int connect_direction)
{
	if (RIGHT == connect_direction)
	{
		(parent)->right = child;
		(child)->parent = parent;
	}
	else
	{
		(parent)->left = child;
		(child)->parent = parent;
	}
}

static void RemoveLeafIMP(bst_iter_t iter)
{
	if(IsLeftChildIMP(iter))
	{
		(iter->parent)->left = NULL;
	}
	else
	{
		(iter->parent)->right = NULL;			
	}
}

static void RemoveNodeWithOneChildIMP(bst_iter_t iter)
{
	if(HasRightChildIMP(iter) && !(HasLeftChildIMP(iter)))
	{
		if(IsRightChildIMP(iter))
		{
			ConnectChildToParentIMP(iter->parent, iter->right, RIGHT);			
		}
		else
		{
			ConnectChildToParentIMP(iter->parent, iter->right, LEFT);							
		}

	}
	else if(!HasRightChildIMP(iter) && (HasLeftChildIMP(iter)))
	{
		if(IsRightChildIMP(iter))
		{
			ConnectChildToParentIMP(iter->parent, iter->left, RIGHT);
		}
		else
		{
			ConnectChildToParentIMP(iter->parent, iter->left, LEFT);	
		}
	}
}

static void RemoveNodeWithTwoChildrenIMP(bst_iter_t iter)
{
	bst_iter_t position_to_insert_subtree = NULL;
	
	if(IsLeftChildIMP(iter))
	{	
		position_to_insert_subtree = BSTPrev(iter);

		ConnectChildToParentIMP(position_to_insert_subtree, iter->right, RIGHT);	
		
		ConnectChildToParentIMP(iter->parent,iter->left, LEFT);		
	}
		
	else
	{
		position_to_insert_subtree = BSTNext(iter);
		
		ConnectChildToParentIMP(position_to_insert_subtree, iter->left, LEFT);		
		
		ConnectChildToParentIMP(iter->parent, iter->right, RIGHT);				
	}
}

static void UpdateParentAndPositionIMP(bst_iter_t **position_for_new_node,
			bst_iter_t *new_node_parent, bst_iter_t *address_of_new_node,
			bst_iter_t parent_node)
{	
	*position_for_new_node = address_of_new_node;
	*new_node_parent = parent_node;								
}

static void DestroryIterIMP(bst_iter_t *iter)
{
	free(*iter);
	*iter = NULL;
}

static void HandleIterInDestroyIMP(bst_iter_t *iter, int direction)
{
	if (RIGHT == direction)
	{
		if(IsLeafIMP((*iter)->right))
		{
			DestroryIterIMP(&(*iter)->right);
		}
		else
		{
			*iter = MoveToIMP(*iter, GO_RIGHT);
		}
	}
	else
	{
		if(IsLeafIMP((*iter)->left))
		{
			DestroryIterIMP(&(*iter)->left);
		}
		else
		{
			*iter = MoveToIMP(*iter, GO_LEFT);
		}
	}
}

static void FindPositionToInsert(bst_iter_t current_node, 
			bst_iter_t *new_node_parent,
			bst_iter_t **position_for_new_node, void *data,bst_t *tree)
{
	int comparison_func_result = 0;
	while (NULL != current_node)
	{				
		comparison_func_result = tree->comparison_func_ptr(
							data , current_node->data, tree->comparison_param);
							
		if (1 == comparison_func_result)
		{
			UpdateParentAndPositionIMP(position_for_new_node, new_node_parent, 
									&(current_node->right),current_node);			
			current_node = MoveToIMP(current_node, GO_RIGHT);
		}
		else
		{
			UpdateParentAndPositionIMP(position_for_new_node, new_node_parent, 
									&(current_node->left),current_node);									
			current_node = MoveToIMP(current_node, GO_LEFT);			
		}
	}
}
static int Count(void *data, void *counter)
{
	UNUSED(data);
	++(*((int *)counter));
	
	return 0;	
}


