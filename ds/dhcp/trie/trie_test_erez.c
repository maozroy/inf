/****************************************************************************
*                                                                           *
*                               OL79 - 17/11/19                             *
*                               Auther - Erez                               *	
*                               Uniqe Identifier Def                        *
*                               Reviewer -??? 	                            *
*****************************************************************************/

#include <string.h> /* memset */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <unistd.h> /* getpid */

#include "../ip/ip.h"
#include "trie.h"
#include "../my_utils.h"

#define BYTE_SIZE (8)

void TestCreateDestroy();
void TestInsert();
void TestSizeAndFullInsert();
void TestDealloc();
void TestSizeAndFullDealloc();

/****************************************************/
int main()
{
	TestCreateDestroy();
	TestInsert();
	TestSizeAndFullInsert();
	TestDealloc();
	TestSizeAndFullDealloc();

	return (0);
}
/************************/
void TestCreateDestroy()
{
	trie_t *my_tree = TrieCreate(2);

	printf("********** Test TrieCreate & TrieDestroy **********\n\n");	

	TrieDestroy(my_tree);
}
/************************/
void TestInsert()
{
	trie_t *my_tree = TrieCreate(3);
	unsigned int *result = (unsigned int*)malloc(sizeof(unsigned int));
	trie_alloc_status_t status = T_IP_NOT_FOUND;
	int test = 1;
	unsigned int allocate = 0; 

	printf("********** Test Insert **********\n\n");	

	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);

/*	RUN_TEST(T_REQUESTED_IP_OCCUPIED == status && allocate == *result," Insert 1 OK");
*/
	++ allocate;
	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);

/*	RUN_TEST(T_REQUESTED_IP_OCCUPIED == status && allocate == *result," Insert 2 OK");	
*/
	++allocate;
	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);

/*	RUN_TEST(T_REQUESTED_IP_OCCUPIED == status && allocate == *result," Insert 3 OK");
*/
	++allocate;
	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);

	++allocate;
	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);\

	++allocate;
	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);

	++allocate;
	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);

	++allocate;
	status = TrieInsert(my_tree, allocate, result);
	(T_SUCCESS_ALLOCATED_REQUESTED == status && allocate == *result) ? (test *= 1) : (test = 0);
	
	status = TrieInsert(my_tree, allocate, result);
	(T_REQUESTED_IP_OCCUPIED == status && allocate == *result) ? (test *= 1) : (test = 0);

	RUN_TEST(T_REQUESTED_IP_OCCUPIED == status && allocate == *result," Insert OK");


	free(result);
	TrieDestroy(my_tree);
}
/************************/
void TestSizeAndFullInsert()
{
	trie_t *my_tree = TrieCreate(3);
	unsigned int *result = (unsigned int*)malloc(sizeof(unsigned int));
	int test = 1;
	unsigned int allocate = 0; 

	printf("********** Test Size & Full For Insert **********\n\n");	

	(0 == TrieCountAlloc(my_tree) && 
	 8 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(1 == TrieCountAlloc(my_tree) && 
	 7 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(1 == TrieCountAlloc(my_tree) && 
	 7 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	++allocate;
	TrieInsert(my_tree, allocate, result);

	(2 == TrieCountAlloc(my_tree) && 
	 6 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(2 == TrieCountAlloc(my_tree) && 
	 6 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	++allocate;
	TrieInsert(my_tree, allocate, result);

	(3 == TrieCountAlloc(my_tree) && 
	 5 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(3 == TrieCountAlloc(my_tree) && 
	 5 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);


	++allocate;
	TrieInsert(my_tree, allocate, result);

	(4 == TrieCountAlloc(my_tree) && 
	 4 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(4 == TrieCountAlloc(my_tree) && 
	 4 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	++allocate;
	TrieInsert(my_tree, allocate, result);

	(5 == TrieCountAlloc(my_tree) && 
	 3 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(5 == TrieCountAlloc(my_tree) && 
	 3 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	++allocate;
	TrieInsert(my_tree, allocate, result);

	(6 == TrieCountAlloc(my_tree) && 
	 2 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(6 == TrieCountAlloc(my_tree) && 
	 2 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	++allocate;
	TrieInsert(my_tree, allocate, result);

	(7 == TrieCountAlloc(my_tree) && 
	 1 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(7 == TrieCountAlloc(my_tree) && 
	 1 == TrieCountFree(my_tree) &&
	 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	++allocate;
	TrieInsert(my_tree, allocate, result);

	(8 == TrieCountAlloc(my_tree) && 
	 0 == TrieCountFree(my_tree) &&
	 1 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	TrieInsert(my_tree, allocate, result);

	(8 == TrieCountAlloc(my_tree) && 
	 0 == TrieCountFree(my_tree) &&
	 1 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);

	RUN_TEST(test," Size & Full For Insert");

	free(result);
	TrieDestroy(my_tree);
}
/************************/
void TestDealloc()
{
	trie_t *my_tree = TrieCreate(3);
	unsigned int *result = (unsigned int*)malloc(sizeof(unsigned int));
	trie_free_status_t status = T_IP_NOT_FOUND;
	int test = 1;
	unsigned int i = 0;

	printf("********** Test Dealloc **********\n\n");	

	for (i = 0; i < 8; ++i)
	{
		TrieInsert(my_tree, i, result);
	}

	for (i = 0; i < 8; ++i)
	{
		status = TrieDeallocate(my_tree, i);
		(status == T_SUCCESS) ? (test *= 1) : (test = 0);	
		status = TrieDeallocate(my_tree, i);
		(status == T_DOUBLE_FREE) ? (test *= 1) : (test = 0);
	}

	RUN_TEST(test, "Dealloc")

	free(result);
	TrieDestroy(my_tree);
}
/************************/
void TestSizeAndFullDealloc()
{
	trie_t *my_tree = TrieCreate(3);
	unsigned int *result = (unsigned int*)malloc(sizeof(unsigned int));
	int test = 1;
	unsigned int i = 0;

	printf("********** Test Size And Full Dealloc **********\n\n");	

	for (i = 0; i < 8; ++i)
	{
		TrieInsert(my_tree, i, result);
	}

	for (i = 0; i < 8; ++i)
	{
		TrieDeallocate(my_tree, i);
		TrieDeallocate(my_tree, i);
		(8-i-1 == TrieCountAlloc(my_tree) && 
		 i+1 == TrieCountFree(my_tree) &&
		 0 == TrieIsFull(my_tree)) ? (test *= 1) : (test = 0);
	}

	RUN_TEST(test, "Size And Full Dealloc")

	free(result);
	TrieDestroy(my_tree);
}
