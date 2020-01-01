#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "avl.h"

enum {LEFT = 0, RIGHT = 1};

typedef struct avl_node avl_node_t;

struct avl_node
{
    struct avl_node *children[2];
    void *data;
    size_t height;
};

struct avl
{
    comparison_func_t comparison_func_t;
    struct avl_node *root;
};

/******************************************************************************/

void RecDestroy(avl_node_t *curr_root);
static avl_node_t *RecInsert(avl_node_t *root, void *data, 
                                    comparison_func_t func);
static avl_node_t *CreateNode(void *data, avl_node_t *r_child, 
                              avl_node_t *l_child, size_t hieght);
static int GetDirection(int cmp_result);
static void *RecAVLFind(const avl_node_t *node_to_find, 
                        const void *data, comparison_func_t CmpFunc);
static void UpdateHeight(avl_node_t *node);
static size_t GetMaxHeight(size_t left_height, size_t right_height);
static int HasTwoChildren(avl_node_t *node);
static int HasOneChild(avl_node_t *node);
static int Isleaf(avl_node_t *node);
static void RecAVLSize(avl_node_t *curr_root, size_t *size);
static int RecForEach(avl_node_t *curr_root, action_func_t func, void *param);
static avl_node_t *GetMostLeft(avl_node_t *node);
static void SwapData(void **data1, void **data2);
static avl_node_t *RecRemove(avl_node_t *node, void *data, 
                             comparison_func_t CmpFunc);
static avl_node_t *RecRemoveTarget(avl_node_t *node, comparison_func_t CmpFunc);
static avl_node_t *GetChild(avl_node_t *node, int direction);
size_t GetHeight(avl_node_t *node);
static avl_node_t *Balance(avl_node_t *node);
static int BalanceFactor(avl_node_t *node);
static int RotationDirection(int balance_factor);
static int IsSingleDirection(avl_node_t *head, int direction, 
                                               int balance_factor);
static avl_node_t *Rotate(avl_node_t *node, int direction);

/******************************************************************************/

avl_t *AVLCreate(comparison_func_t func)
{
    avl_t *avl = (avl_t *)malloc(sizeof(avl_t));
    if (NULL == avl)
    {
        return NULL;
    }

    avl->comparison_func_t = func;
    avl->root = NULL;

    return avl;
}

void AVLDestroy(avl_t *tree)
{
    avl_node_t *curr_root = tree->root;

    assert(NULL != tree);

    RecDestroy(curr_root);
    free(tree);
}

void RecDestroy(avl_node_t *curr_root)
{
    if (NULL == curr_root)
    {
        return;
    }

    RecDestroy(curr_root->children[LEFT]);
    RecDestroy(curr_root->children[RIGHT]);

    curr_root->children[LEFT] = NULL;
    curr_root->children[RIGHT] = NULL;
    free(curr_root);
}

/******************************************************************************/

int AVLInsert(avl_t *tree, void *data)
{
    avl_node_t *curr_root = NULL;

    assert(NULL != tree);

    curr_root = tree->root;

    curr_root = RecInsert(curr_root, data, tree->comparison_func_t);
    if (NULL == curr_root)
    {
        return 1;
    }

    tree->root = curr_root;

    return 0;
}

/******************************************************************************/

static avl_node_t *RecInsert(avl_node_t *root, void *data, comparison_func_t func)
{
    int cmp_result = 0;
    int direction = 0;

    assert(NULL != func);

    if (NULL == root)
    {
        root = (avl_node_t *)CreateNode(data, NULL, NULL, 0);
        if (NULL == root)
        {
            return NULL;
        }

        return root;
    }

    cmp_result = func(data, root->data);
    direction = GetDirection(cmp_result);
    root->children[direction] = RecInsert(root->children[direction], data, func);
    
    UpdateHeight(root);
    root = Balance(root);
    
    return root;
}

static avl_node_t *CreateNode(void *data, avl_node_t *r_child, 
                              avl_node_t *l_child, size_t hieght)
{
    avl_node_t *node = (avl_node_t *)malloc(sizeof(avl_node_t));
    if (NULL == node)
    {
        return NULL;
    }

    node->children[LEFT] = l_child;
    node->children[RIGHT] = r_child;
    node->data = data;
    node->height = hieght;

    return node;
}

static int GetDirection(int cmp_result)
{
    return (1 == cmp_result) ? RIGHT : LEFT;
}

static void UpdateHeight(avl_node_t *node)
{
    size_t l_child_height = 0;
    size_t r_child_height = 0;

    assert(NULL != node);

    if (HasOneChild(node))
    {
        node->height = GetChild(node, LEFT)->height + 1;
    }
    else if (HasTwoChildren(node))
    {
        l_child_height = GetHeight(GetChild(node, LEFT));
        r_child_height = GetHeight(GetChild(node, RIGHT));
        node->height = GetMaxHeight(l_child_height, r_child_height) + 1;
    }
    else if (Isleaf(node))
    {
        node->height = 0;
    }
}

static size_t GetMaxHeight(size_t left_height, size_t right_height)
{
    if (left_height > right_height)
    {
        return left_height;
    }

    return right_height;
}

size_t GetHeight(avl_node_t *node)
{
    if (NULL == node)
    {
        return 0;
    }
    
    return node->height;
}

static int HasOneChild(avl_node_t *node)
{
    assert(NULL != node);

    return (!HasTwoChildren(node) && !Isleaf(node));
}

static int HasTwoChildren(avl_node_t *node)
{
    assert(NULL != node);

    return ((NULL != node->children[LEFT]) && (NULL != node->children[RIGHT]));
 
}
static int Isleaf(avl_node_t *node)
{
    assert(NULL != node);

    return ((NULL == node->children[LEFT]) && (NULL == node->children[RIGHT]));
}

/******************************************************************************/

void AVLRemove(avl_t *tree, const void *data)
{
    avl_node_t *node = NULL;

    assert(NULL != tree);

    node = tree->root;

    tree->root = RecRemove(node, (void *)data, tree->comparison_func_t);
}

static avl_node_t *RecRemove(avl_node_t *node, void *data, 
                                                        comparison_func_t CmpFunc)
{
    int cmp_result = 0;
    int direction = 0;

    if (NULL == node)
    {
        return NULL;
    }

    cmp_result = CmpFunc(data, node->data);
    direction = GetDirection(cmp_result);
    if (0 == cmp_result)
    {
        return RecRemoveTarget(node, CmpFunc);
    }

    node->children[direction] = RecRemove(node->children[direction], 
                                                     data, CmpFunc);
    UpdateHeight(node);
    node = Balance(node);
 
    return node;
}

static avl_node_t *RecRemoveTarget(avl_node_t *node, comparison_func_t CmpFunc)
{
    avl_node_t *node_child = NULL;
    avl_node_t *node_to_replace = NULL;
    avl_node_t *node_to_remove = NULL;
    void *new_root_data = NULL;

    assert(NULL != node);

    if (Isleaf(node))
    {
        node->children[LEFT] = NULL;
        node->children[RIGHT] = NULL;
        free(node);

        return NULL;
    }

    if (HasOneChild(node))
    {
        node_child = GetChild(node, LEFT);
        node->children[LEFT] = NULL;
        node->children[RIGHT] = NULL;
        free(node);

        return node_child;        
    }

    if (HasTwoChildren(node))
    {
        node_to_replace = GetMostLeft(node->children[RIGHT]);
        new_root_data = node_to_replace->data;
        node_to_remove = node;
        node = RecRemove(node, node_to_replace->data, CmpFunc);
        SwapData(&node_to_remove->data, &new_root_data);
        UpdateHeight(node);        
    }
    
    node = Balance(node);

    return node;
}

static avl_node_t *GetMostLeft(avl_node_t *node)
{
    assert(NULL != node);

    if (NULL == node->children[LEFT])
    {
        return node;
    }

    return GetMostLeft(node->children[LEFT]);
}

static void SwapData(void **data1, void **data2)
{
    void *tmp = NULL;

    assert(NULL != data1);
    assert(NULL != data2);

    tmp = *data1;
    *data1 = *data2;
    *data2 = tmp;
}

static avl_node_t *GetChild(avl_node_t *node, int direction)
{
    assert(NULL != node);

    if (NULL != node->children[direction])
    {
        return node->children[direction];
    }

    return node->children[!direction];
}

void DestroyNode(avl_node_t *node)
{
    if (NULL != node)
    {
        node->children[LEFT] = NULL;
        free(node);

    }
}


/******************************************************************************/

static avl_node_t *Balance(avl_node_t *node)
{
    int balance_factor = 0;
    int direction = 0;

    assert(NULL != node);

    balance_factor = BalanceFactor(node);
    if (abs(balance_factor) > 1)
    {
        direction = RotationDirection(balance_factor);
        if (IsSingleDirection(node, !direction, balance_factor))
        {
            node = Rotate(node, direction);
        }
        else
        {
            node->children[!direction] = Rotate(node->children[!direction], 
                                                               !direction);
            node = Rotate(node, direction);
        }

        UpdateHeight(node);
    }

    return node;
}

static int BalanceFactor(avl_node_t *node)
{
    size_t h_left = 0;
    size_t h_right = 0;

    assert(NULL != node);

    if (NULL == node->children[LEFT])
    {
        h_left = 0;
    }
    else
    {
        h_left = GetHeight(node->children[LEFT]) + 1;
    }

    if (NULL == node->children[RIGHT])
    {
        h_right = 0;
    }
    else
    {
        h_right = GetHeight(node->children[RIGHT]) + 1;
    }
    
    return ((int)(h_left - h_right));
}

static int RotationDirection(int balance_factor)
{
    return ((balance_factor > 1) ? RIGHT : LEFT);
}

static int IsSingleDirection(avl_node_t *head, int direction, int balance_factor)
{
    int child_balance_factor = 0;

    assert(NULL != head);
    assert(NULL != head->children[direction]);

    child_balance_factor = BalanceFactor(head->children[direction]);

    return ((child_balance_factor > 0 && balance_factor > 0) || 
           (child_balance_factor < 0 && balance_factor < 0));
}

static avl_node_t *Rotate(avl_node_t *node, int direction)
{
    avl_node_t *new_parent = NULL;

    assert(NULL != node);

    new_parent = node->children[!direction];
    node->children[!direction] = new_parent->children[direction];
    new_parent->children[direction] = node;

    UpdateHeight(node);

    return new_parent;
}

/******************************************************************************/
size_t AVLGetHeight(const avl_t *tree)
{
    assert(NULL != tree);

    if (AVLIsEmpty(tree))
    {
        return 0;
    }

    return tree->root->height;
}

/******************************************************************************/

void *AVLFind(const avl_t *tree, const void *data)
{
    avl_node_t *node_to_find = NULL;

    assert(NULL != tree);

    node_to_find = tree->root;

    return RecAVLFind(node_to_find, data, tree->comparison_func_t);
}

static void *RecAVLFind(const avl_node_t *node_to_find, 
                        const void *data, comparison_func_t CmpFunc)
{   
    int cmp_result = 0;
    int direction = 0;

    assert(NULL != CmpFunc);

    if (NULL == node_to_find)
    {
        return NULL;
    }
    
    cmp_result = CmpFunc(data, node_to_find->data);
    if (0 == cmp_result)
    {
        return (void *)data;
    }

    direction = GetDirection(cmp_result);

    return RecAVLFind(node_to_find->children[direction], data, CmpFunc);
}

/******************************************************************************/

int AVLIsEmpty(const avl_t *tree)
{
    assert(NULL != tree);

    return (NULL == tree->root);
}

/******************************************************************************/

size_t AVLSize(const avl_t *tree)
{
    avl_node_t *curr_root = NULL;
    size_t size = 0;

    assert(NULL != tree);

    curr_root = tree->root;

    RecAVLSize(curr_root, &size);

    return size;
}

static void RecAVLSize(avl_node_t *curr_root, size_t *size)
{
    if (NULL == curr_root)
    {
        return;
    }

    ++(*size);

    RecAVLSize(curr_root->children[LEFT], size); 
    RecAVLSize(curr_root->children[RIGHT], size);
}

/******************************************************************************/

int AVLForEach(avl_t *tree, action_func_t func, void *param)
{
    avl_node_t *curr_root = NULL;

    assert(NULL != tree);
    assert(NULL != func);

    curr_root = tree->root;
    return RecForEach(curr_root, func, param);
}

static int RecForEach(avl_node_t *curr_root, action_func_t func, void *param)
{
    int result = 0;

    assert(NULL != func);

    if (NULL == curr_root)
    {
        return 0;
    }

    RecForEach(curr_root->children[LEFT], func, param);

    result = func(curr_root->data, param);
    if (0 != result)
    {
        return result;
    }

    result = RecForEach(curr_root->children[RIGHT], func, param);

    return result;
}

