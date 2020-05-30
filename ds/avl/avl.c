 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	Erez Barr
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

typedef enum direction
{
	NONE = -1,
	LEFT = 0,
	RIGHT = 1,
	BOTH = 2
}direction;

enum status
{
	SUCCESS = 0,
	FAILURE = 1
};

enum func_result
{
	MATCH = 0,
	GO_RIGHT = 1,
	GO_LEFT = -1
};

static direction GetDirectionIMP(direction dir);
static direction GetOppositeIMP(direction);
static avl_node_t *RecInsertIMP(avl_node_t **node, 
								comparison_func_t func, void *data);
static avl_node_t **GetRootAdrsIMP(avl_t *tree);
static void UpdateHeightIMP(avl_node_t *node);
static void RecDestroyIMP(avl_node_t *node);
static avl_node_t *CreateNodeIMP(void *data);
static void RecSizeIMP(avl_node_t *node, size_t *size);
static size_t MaxIMP(size_t a, size_t b);
static int GetNumOfKidsIMP(avl_node_t *node);
static avl_node_t *RecFindIMP(avl_node_t *node, comparison_func_t func, 
							void *data);
static int RecForEachIMP(avl_node_t *node, action_func_t func, void* param);
static void RecRemoveIMP(avl_node_t *node, void *data, avl_t *tree);
static void ConnectToLeftChildIMP(avl_node_t *node, 
									avl_node_t *node_to_connect);
static direction IsMyChildTheChosenOneIMP
		(avl_node_t *node, void *data, comparison_func_t func);
static void RemoveByParentIMP(avl_node_t *node_to_remove,direction direction);
static void RemoveRootIMP(avl_node_t *node, avl_t *tree);
static size_t GetHeightIMP(avl_node_t *node);
/*balancing funcs*/
static avl_node_t *AVLBalanceIMP(avl_node_t *node);
static void TwoDegreeRotationIMP(avl_node_t *node, int dir);
static avl_node_t *OneDegreeRotationIMP(avl_node_t *node, int dir);
static size_t GetDifferenceIMP(avl_node_t *node);

avl_t *AVLCreate(comparison_func_t func)
{
	avl_t *new_tree = (avl_t *)malloc(sizeof(avl_t));
	if (NULL == new_tree)
	{
		return NULL;
	}
	
	assert(func);
	
	new_tree -> root = NULL;
	new_tree -> comparison_func = func;

	return new_tree;
}

int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t **root = NULL;
	avl_node_t *new_node = NULL;
	
	assert(data);
	assert(tree);
	
	root = GetRootAdrsIMP(tree);
	new_node = RecInsertIMP(root, tree -> comparison_func, data);
	if (NULL == new_node)
	{
		return FAILURE;
	}
	
	return SUCCESS;
}

size_t AVLSize(const avl_t *tree)
{
	size_t size = 0;
	avl_node_t *root = tree -> root;
	if (NULL == root)
	{
		return 0;
	}
	
	RecSizeIMP(root, &size);

	return size;
}

void *AVLFind(const avl_t *tree, const void *data)
{
	avl_node_t *node = NULL;
	
	assert(data);
	assert(tree);
	
	node = RecFindIMP(tree -> root, tree -> comparison_func, (void *)data);
	if (NULL == node)
	{
		return NULL;
	}

	return node -> data;
}

int AVLForEach(avl_t *tree, action_func_t func, void *param)
{
	assert(tree);
	assert(func);

	return RecForEachIMP(tree -> root, func, param);
}

void AVLRemove(avl_t *tree, const void *data)
{
	assert(tree);

	if (NULL == data)
	{
		return;
	}
	RecRemoveIMP(tree -> root, (void *)data, tree);
}

void AVLDestroy(avl_t *tree)
{
	assert(tree);

	RecDestroyIMP(*(GetRootAdrsIMP(tree)));
	
	free(tree);
}

size_t AVLGetHeight(const avl_t *tree)
{
	assert(tree);

	if (NULL == tree -> root)
	{
		return 0;
	}

	return tree -> root -> height;
}

int AVLIsEmpty(const avl_t *tree)
{
	assert(tree);

	return (tree -> root == NULL);
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
	new_node -> data = data;
	new_node -> height = 0;
	new_node -> children[LEFT] = NULL;
	new_node -> children[RIGHT] = NULL;
	
	return new_node;
}

static size_t GetHeightIMP(avl_node_t *node)
{
	if (NULL == node)
	{
		return NONE;
	}
	return node -> height;
}

static size_t GetDifferenceIMP(avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	return (GetHeightIMP(node -> children[RIGHT]) - 
			GetHeightIMP(node -> children[LEFT]));
}

static avl_node_t *OneDegreeRotationIMP(avl_node_t *node, int dir)
{
	avl_node_t *pivot = node -> children[dir];
	
	node->children[dir] = node -> children[dir] ->
								children[GetOppositeIMP(dir)];					
	pivot -> children[GetOppositeIMP(dir)] = node;
	UpdateHeightIMP(node);
	
	return pivot;
}

static void TwoDegreeRotationIMP(avl_node_t *node, int dir)
{
	avl_node_t *pivot = node -> children[dir];
	avl_node_t *node_to_connect = node -> children[dir] -> 
									children[GetOppositeIMP(dir)];
	node->children[dir] = node_to_connect;
	pivot->children[GetOppositeIMP(dir)] = node_to_connect->children[dir];
	node_to_connect->children[dir] = pivot;
	UpdateHeightIMP(pivot);
	UpdateHeightIMP(node_to_connect);
	
	return;
}

static enum direction GetOppositeIMP(enum direction direction)
{
	if (direction == RIGHT)
	{
		return LEFT;
	}
	if (direction == LEFT)
	{
		return RIGHT;
	}
	return NONE;
}

static avl_node_t *AVLBalanceIMP(avl_node_t *node)
{
	avl_node_t *pivot = NULL;
	int main_rotation = GetDifferenceIMP(node);
	int small_rotation = 0;

	if (main_rotation > 1)
	{
		main_rotation = RIGHT;
		small_rotation = GetDifferenceIMP(node->children[main_rotation]);
		
		if (small_rotation >= 0)
		{
			pivot = OneDegreeRotationIMP(node, main_rotation);
		}
		else
		{
			TwoDegreeRotationIMP(node, main_rotation);
			pivot = OneDegreeRotationIMP(node, main_rotation);
		}
	}
	else if (main_rotation < -1)
	{
		main_rotation = LEFT;
		small_rotation = GetDifferenceIMP(node->children[main_rotation]);
		if (small_rotation <= 0)
		{
			pivot = OneDegreeRotationIMP(node, main_rotation);
		}
		else
		{
			TwoDegreeRotationIMP(node, main_rotation);
			pivot = OneDegreeRotationIMP(node, main_rotation);
		}
	}
	else
	{
		return node;
	}
	return pivot;
}

static direction GetDirectionIMP(direction dir)
{
	if ((direction)GO_LEFT == dir)
	{
		return LEFT;
	}

	return dir;
}

static avl_node_t **GetRootAdrsIMP(avl_t *tree)
{
	return &(tree->root);
}

static void UpdateHeightIMP(avl_node_t *node)
{
	int result = 0;
	
	if (NULL == node)
	{
		return;
	}
	
	switch (GetNumOfKidsIMP(node))
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
	node->height = result;
}

static void RecSizeIMP(avl_node_t *node, size_t *size)
{	
	if (NULL == node)
	{
		return;
	}
	
	(*size) += 1;
	RecSizeIMP(node->children[LEFT], size);
	RecSizeIMP(node->children[RIGHT], size);

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

static avl_node_t *RecFindIMP(avl_node_t *node, 
								comparison_func_t func, void *data)
{
	int cmp_result = 0;
	
	if (node == NULL)
	{
		return NULL;
	}
	
	cmp_result = func(data, node->data);
	if (MATCH == cmp_result)
	{
		return node;
	}
	
	return RecFindIMP(node->children[GetDirectionIMP(cmp_result)], func, data);
}

static enum direction IsMyChildTheChosenOneIMP
						(avl_node_t *node, void *data, comparison_func_t func)
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
	
	if (MATCH == cmp_result_right)
	{
		return RIGHT;
	}
	if (MATCH == cmp_result_left)
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

	if (NULL == node)
	{
		return;
	}
	
	cmp_result = func(data, node->data);
	if (MATCH == cmp_result)
	{
		RemoveRootIMP(node, tree);
		tree->root = AVLBalanceIMP(tree->root);
		UpdateHeightIMP(tree->root);
		
		return;
	}
	
	direction = IsMyChildTheChosenOneIMP(node, data, func);
	if (direction != NONE)
	{
		RemoveByParentIMP(node, direction);
	}
	else
	{
		direction = GetDirectionIMP(cmp_result);
		RecRemoveIMP(node -> children[direction],data, tree);
		
		node -> children[direction] = AVLBalanceIMP
										(node -> children[direction]);
		UpdateHeightIMP(node);
	}
	UpdateHeightIMP(node);

	return;
}

static void RemoveRootIMP(avl_node_t *node, avl_t *tree)
{
	switch (GetNumOfKidsIMP(node))
	{
		case NONE:
			tree -> root = NULL;
			break;
			
		case RIGHT:
			tree -> root = node -> children[RIGHT];
			break;
			
		case LEFT:
			tree -> root = node -> children[LEFT];
			break;
			
		case BOTH:
			ConnectToLeftChildIMP(node->children[RIGHT], 
									node -> children[LEFT]);
			tree -> root = node -> children[RIGHT];
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
			node -> children[direction] = NULL;
			break;
		case RIGHT:
			node -> children[direction] = node_to_remove -> children[RIGHT];
			break;
		case LEFT:
			node -> children[direction] = node_to_remove -> children[LEFT];
			break;
		case BOTH:
			ConnectToLeftChildIMP(node_to_remove -> children[RIGHT], 
									node_to_remove -> children[LEFT]);
			node -> children[direction] = node_to_remove -> children[RIGHT];
			break;
	}
	free(node_to_remove);
}

static void ConnectToLeftChildIMP(avl_node_t *node, avl_node_t *node_to_connect)
{
	if (NULL == node -> children[LEFT])
	{
		node -> children[LEFT] = node_to_connect;
		UpdateHeightIMP(node);
		
		return;
	}
	ConnectToLeftChildIMP(node -> children[LEFT], node_to_connect);
	UpdateHeightIMP(node);
	
	return;
}


static int RecForEachIMP(avl_node_t *node, action_func_t func, void* param)
{
	int func_res = 0;
	
	if (NULL == node)
	{
		return SUCCESS;
	}

	RecForEachIMP(node -> children[LEFT], func, param);

	func_res = func(node -> data, param);
	if (SUCCESS != func_res)
	{
		return func_res;
	}

	return RecForEachIMP(node -> children[RIGHT], func, param);
}

static avl_node_t *RecInsertIMP(avl_node_t **node, 
								comparison_func_t func, void *data)
{
	int cmp_result = 0;
	int dir = 0;
	
	if (NULL == *node)
	{
		*node = CreateNodeIMP(data);
	}
	else
	{
		cmp_result = func(data, (*node) -> data);
		dir = GetDirectionIMP(cmp_result);
		(*node) -> children[dir] = RecInsertIMP
								(&((*node) -> children[dir]), func, data);
	}
	(*node) = AVLBalanceIMP(*node);
	UpdateHeightIMP(*node);
	
	return *node;
}

static void RecDestroyIMP(avl_node_t *node)
{
	if (node != NULL)
	{
		RecDestroyIMP(node -> children[LEFT]);
		RecDestroyIMP(node -> children[RIGHT]);
		
		node -> children[LEFT] = NULL;
		node -> children[RIGHT] = NULL;
		
		free(node);
		node = NULL;
	}
}
