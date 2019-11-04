/****************************************************************************
*                                                                           *
*                               OL79 - 04/11/19                             *
*                                                                           *	
*                                   SList                                   *
*                                                                           *
*****************************************************************************/
#include <stdio.h>
#ifndef SLIST_H
#define SLIST_H

typedef struct node 
{
	void *data;
	struct node *next;
}node_t;

typedef int (*find_ptr)(const node_t *node, void *param);
typedef int (*for_each_ptr)(node_t *node, void *param);

/* The function creates a node.
 * The function recieves a pointer to next node, and a pointer to the data. 
 * If next is NULL - node will not connect to anything*/
node_t *SListCreateNode(void *data, node_t *next);

/*The function destroyes all nodes, from node (including node) 
 * and onward until NULL
 * Notice - if the nodes are in loop, the function will result in undefined
 * behavior 
 */
void SListFreeNode(node_t *node);

/*The function inserts a new node before pos. the function also connects to the 
 *  node that was before pos 
 * The function return a pointer to new_node.
 */
node_t *SListInsert(node_t *new_node, node_t *pos);

/*The function inserts a new node after pos
 *The function return a pointer to new_node.
 */
node_t *SListInsertAfter(node_t *new_node, node_t *pos);

/* The function counts the number of nodes, from node (including node) 
 * and onward until NULL.
 * Notice - if the nodes are in loop, the function will result in undefined
 * behavior */
size_t SListCount(const node_t *head);

/* The function loops over every node, from *node and onward, and passes it,
 * along side param, to the function that is pointed by ptr. 
 * The ptr function should look for param in the node that is passed to it. 
 * If ptr finds a match, it should return 1.
 * If ptr found a match, The SListFind returns a pointer to the matched node.
 * else - it will return null.
 * Notice - if the nodes are in loop, the function will result in undefined
 * behavior */
 node_t *SListFind(const node_t *node, void *param, find_ptr ptr);

/* The function removes the node passed to the function 
 * The function returns the node that was after the removed node. */
node_t *SListRemove(node_t *node);

/* The function removes the node that is after the node that was passed
 * to the function.
 * The function returns the node that was passed to the function. */
node_t *SListRemoveAfter(node_t *node);

/* The function reverses the order of the nodes, from node and onward,
 * untill null. 
 * The function returns the new 'head', which is the node that was the last.
 *  Notice - if the nodes are in loop, the function will result in undefined
 * behavior */
node_t *SListFlip(node_t *head);

/* The function loops over every node, from *node and onward, and passes it,
 * along side param, to the function that is pointed by ptr. the ptr function 
 * can do whatever you want, but it should return 1 if it fails.
 * If the SListForEach functions recieves 1 from ptr, 
 * it will stop the foreach and will return 1.
 * Notice - if the nodes are in loop, the function will result in undefined
 * behavior */
int SListForEach(node_t *node, void *param, for_each_ptr ptr);

/* The function check if the nodes, from node and onward, until null, has a loop.
 *  If there is loop - it will return 1. else - will return 0 */
int SListHasLoop (node_t *head);

/* The function checks if there is an intersection, which means thet two nodes 
 * points to same node. The function returns 1 if there is intersection. else -
 * will return zero */
node_t *SListFindIntersection (node_t *node1, node_t *node2);

#endif
