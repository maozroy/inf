/****************************************************************************
*                                                                           *
*                                Tamir Dayan                                *
*                              OL79 - 00/00/19                              *
*                                                                           *	
*                                Subject                                    *
*                           Reviewer:                                       *
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "trie.h" /* trie */
#include "../myutils.h"

void TestTrieCreate();
void TestTrieDestroy();
void TestTrieInsert();
void TestTrieDeallocate();
void TestTrieIsFull();
void TestTrieCountAlloc();
void TestTrieCountFree();

int main()
{
	TestTrieCreate();
	TestTrieDestroy();
	TestTrieInsert();
	TestTrieDeallocate();
	TestTrieIsFull();
	TestTrieCountAlloc();/*
	TestTrieCountFree();*/
	
	return 0;
}

void TestTrieCreate()
{
	trie_t *trie = NULL;
	 
	printf("\n*****" BBLU "TestTrieCreate" KNRM "*****\n");
	TEST("TestTrieCreate", 1, NULL == trie);
	trie = TrieCreate(1);
	TEST("TestTrieCreate", 2, NULL != trie);
	printf("***** ********* *****\n");
	TrieDestroy(trie);
}

void TestTrieDestroy()
{
	printf("\n*****" BBLU "TestTrieDestroy" KNRM "*****\n");
	printf(BRED "***** CHECK FOR VALGRIND *****" KNRM "\n");
	printf("***** ********* *****\n");
}

void TestTrieInsert()
{
	trie_t *trie = NULL;
	trie = TrieCreate(3);

	printf("\n*****" BBLU "TestTrieInsert" KNRM "*****\n");
	TEST("TestTrieInsert", 1, SUCCESS_ALLOCATED_REQUSTED == TrieInsert(trie, 1));
	TEST("TestTrieInsert", 2, REQUESTED_IP_OCCUPIED == TrieInsert(trie, 1));
	printf("***** ********* *****\n");
	TrieDestroy(trie);
}

void TestTrieDeallocate()
{
	trie_t *trie = NULL;
	trie = TrieCreate(3);

	printf("\n*****" BBLU "TestTrieDeallocate" KNRM "*****\n");
	TrieInsert(trie, 1);
	TEST("TestTrieDeallocate", 1, IP_NOT_FOUND == TrieDeallocate(trie, 2));
	TEST("TestTrieDeallocate", 2, SUCCESS == TrieDeallocate(trie, 1));
	TEST("TestTrieDeallocate", 3, DOUBLE_FREE == TrieDeallocate(trie, 1));
	printf("***** ********* *****\n");
	TrieDestroy(trie);
}

void TestTrieIsFull()
{
	trie_t *trie = NULL;
	trie = TrieCreate(2);

	printf("\n*****" BBLU "TestTrieIsFull" KNRM "*****\n");
	TEST("TestTrieIsFull", 1, 0 == TrieIsFull(trie));
	TrieInsert(trie, 0);
	TEST("TestTrieIsFull", 2, 0 == TrieIsFull(trie));
	TrieInsert(trie, 1);
	TEST("TestTrieIsFull", 3, 0 == TrieIsFull(trie));
	TrieInsert(trie, 2);
	TEST("TestTrieIsFull", 4, 0 == TrieIsFull(trie));
	TrieInsert(trie, 3);
	TEST("TestTrieIsFull", 5, 1 == TrieIsFull(trie));
	printf("***** ********* *****\n");
	TrieDestroy(trie);
}


void TestTrieCountAlloc()
{
	trie_t *trie = NULL;
	trie = TrieCreate(2);

	printf("\n*****" BBLU "TestTrieCountAlloc" KNRM "*****\n");
	TEST("TestTrieCountAlloc", 1, 0 == TrieCountAlloc(trie));
	TrieInsert(trie, 0);
	TEST("TestTrieCountAlloc", 2, 1 == TrieCountAlloc(trie));
	TrieInsert(trie, 1);
	TEST("TestTrieCountAlloc", 3, 2 == TrieCountAlloc(trie));
	TrieInsert(trie, 2);
	TEST("TestTrieCountAlloc", 4, 3 == TrieCountAlloc(trie));
	TrieInsert(trie, 3);
	TEST("TestTrieCountAlloc", 5, 4 == TrieCountAlloc(trie));
	printf("***** ********* *****\n");
	TrieDestroy(trie);
}
/*
void TestTrieCountFree()
{

}
*/