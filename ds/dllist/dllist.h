/****************************************************************************
*                                                                           *
*                               OL79 - 11/11/19                             *
*                                                                           *	
*                                   DLList                                   *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_DLLIST_H
#define ILRD_DLLIST_H

#include <stddef.h>

typedef struct node dll_node_t;
typedef dll_node_t *dll_iter_t;
typedef struct dl_list dl_list_t;
typedef int (*find_ptr)(const dll_node_t *node, const void *param);
typedef int (*for_each_ptr)(dll_node_t *node, void *param);

/* Function creates a list struct.
   returns pointer to list struct */
dl_list_t *DLListCreate(void);

/*
* The function destroys all nodes in list
*/
void DLListDestroy(dl_list_t *list);

/*Function adds node to the back of the list. 
  Returns node (iterator) to new added node*/
dll_iter_t DLListPushBack(dl_list_t *list, void *data);

/*Function adds node to the front of the list. 
  Returns node (iterator) to new added node*/
dll_iter_t DLListPushFront(dl_list_t *list, void *data);

/*The function inserts a new node before iterator.
 * The function return the new node.
 */
dll_iter_t DLListInsert(void *data, dll_iter_t iterator, dl_list_t *list);

/* The function pops the front node.
 * The function return pointer to the data that was in the node.
 */
void *DLListPopFront(dl_list_t *list);

/* The function pops the back node.
 * The function return pointer to the data that was in the node.
*  If list is empty pop will result in undifined behavior
 */
void *DLListPopBack(dl_list_t *list);

/* Function removes iterator and returns iterrator of the next node*/
dll_iter_t DLListRemove(dll_iter_t iterator);

/*Function merges between two lists. It receives a section of 
one list (begin and end) and destination to which to connect it to.
after merge detination points to begin and end points to the old 
desination -> next. Function returns destination.
The user must verify the that begin -> ... -> end
begin and dest must be of different lists
*/
dll_iter_t DLListSplice(dll_iter_t s_begin, dll_iter_t s_end, dll_iter_t dest);

/* Function receives list.
   Function returns size of list */ 
size_t DLListSize(const dl_list_t *list);

/* Function receives list.
   Function returns 1 if Empty and 0 if not */ 
int DLListIsEmpty(const dl_list_t *list);

/* The function loops over every node, from *node and onward, and passes it,
 * alongside param, to the function that is pointed by ptr. 
 * The ptr function should look for param in the node that is passed to it. 
 * If ptr finds a match, it should return 1.
 * If ptr found a match, The DLListFind returns a pointer to the matched node.
 * else - it will return null.
 * Notice - if the nodes are in loop, the function will result in undefined
 * behavior */
dll_iter_t DLListFind(dll_iter_t begin, dll_iter_t end, 
					  const void *param, find_ptr ptr);

/* The function loops over every node, from *node and onward, and passes it,
 * along side param, to the function that is pointed by ptr. the ptr function
 * can do whatever you want, but it should return 1 if it fails.
 * If the DLListForEach functions recieves 1 from ptr, 
 * it will stop the foreach and will return 1.
 * Notice - if the nodes are in loop, the function will result in undefined
 * behavior */
int DLListForEach(dll_iter_t begin, dll_iter_t end, 
				  void *param, for_each_ptr ptr);

/* Function receives list. Function returns iterator to start of list */
dll_iter_t DLListBegin(const dl_list_t *list);

/* Function receives list. Function returns iterator to end of list */
dll_iter_t DLListEnd(const dl_list_t *list);

/* Function receives iterator. Function returns next iterator
   If iterator is the end node this will result in undifined behavior */
dll_iter_t DLListNext(dll_iter_t iterator);

/* Function receives iterator. Function returns previous iterator
If iterator is the start node this will result in undifined behavior */
dll_iter_t DLListPrev(dll_iter_t iterator);

/* Function receives iterator. Function returns data located inside */
void *DLListGetData(dll_iter_t iterator);

/* Function receives two iterators. Function returns 1 if the are equal 
and 0 if not */
int DLListIsSameIterator(dll_iter_t iterator1, dll_iter_t iterator2);

#endif
