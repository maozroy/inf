/****************************************************************************
*                                                                           *
*                               OL79 - 6/16/19                             *
*                                                                           *	
*                                   Queue                                   *
*                                                                           *
*****************************************************************************/
#ifndef ILRD_QUEUE_H
#define ILRD_QUEUE_H
#include <stddef.h> 

typedef struct queue queue_t;

/* This function creates a queue of any suitable type and inserts data to first place in the queue. 
* Return value: a pointer to queue_t data type. */
queue_t *QCreate();

/* This function destroys a queue which was created only by QCreate. the function also dealloctes the memory that was allocated.
   Notice: calling this function with any other queue will result in 
   undefined behavior. */
void QDestroy(queue_t *queue);

/* This function adds an element to the end of the queue.
*  Return values: 0 - operation succeeded, 1 - operation failed. */
int QEnqueue(queue_t *queue, void *data);

/* This function extracts the first element of the queue.
   Notice: attempting to dequeue from an empty queue will result in 
   undefined behavior. */
void QDequeue(queue_t *queue);

/* This function returns the value of the first element of the queue.
*  Notice: attempting to peek from an empty queue will result in 
   undefined behavior. */
void *QPeek(const queue_t *queue);

/* This function checks if the queue is empty. 
   Return values: 0 - not empty, 1 - empty. */
int QIsEmpty(const queue_t *queue);

/* This function returns the size (number of elements) of the queue */
size_t QSize(const queue_t *queue);

/* returns dst. src will be at the end of dst*/ 
void QAppend(queue_t *dest, queue_t *src); 

#endif
