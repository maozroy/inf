#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */
#include "my_utils.h"
#include "vsa.h"

void TestVSAInit();
void TestVSAAlloc();
void TestVSAFree();
void TestVSALargestChunk();
void TestFlow();

void TestFlowDebug();
void TestVSALargestChunkDebug();
void TestVSAFreeDebug();

void TestVSAFreeDebug()
{
	void *buffer = calloc(sizeof(char),200);
	vsa_t *vsa = VSAInit(buffer, 200);	
	void *result = VSAAlloc(vsa, 17);
	result = VSAAlloc(vsa, 25);
	VSAFree(result);
	
	PRINTTESTRESULTS("VSAFree", 1, (*((char *)vsa + 40) == -32));
	result = VSAAlloc(vsa, 33);
	PRINTTESTRESULTS("VSAFree", 2, (*((char *)vsa + 40) == 40));

	VSAFree(result);
	PRINTTESTRESULTS("VSAFree", 3, (*((char *)vsa + 40) == -40));
	free(buffer);	
}

void TestVSALargestChunkDebug()
{
	void *buffer = calloc(sizeof(char),200);
	vsa_t *vsa = VSAInit(buffer, 200);	
	void *result = VSAAlloc(vsa, 17);
	result = VSAAlloc(vsa, 25);
	VSAFree(result);

	PRINTTESTRESULTS("VSALargestChunkDebug", 1, (VSALargestChunk(vsa) == 128));
/*	printf("%ld\n",VSALargestChunk(vsa));*/
	free(buffer);	
}

void TestFlowDebug()
{
	void *buffer = calloc(sizeof(char),200);
	vsa_t *vsa = VSAInit(buffer, 200);	
	void *result = VSAAlloc(vsa, 24);
	void *result2 = NULL;
	result = VSAAlloc(vsa, 32);
	result = VSAAlloc(vsa, 40);
	result2 = VSAAlloc(vsa, 56);
	/*printf("%d\n",*((char *)vsa + 184));*/
	
	PRINTTESTRESULTS("FlowDebug", 1, (*((char *)vsa + 184) == 0));

	PRINTTESTRESULTS("FlowDebug", 2, (result2 == NULL));

	VSAFree(result);

	PRINTTESTRESULTS("FlowDebug", 3, (*((char *)vsa + 88) == -40));


	free(buffer);	
}

void TestFlow()
{
	void *buffer = malloc(200);
	vsa_t *vsa = VSAInit(buffer, 200);	
	void *result = VSAAlloc(vsa, 24);
	void *result2 = NULL;
	result = VSAAlloc(vsa, 32);
	result = VSAAlloc(vsa, 40);
	VSAAlloc(vsa, 56);
	
	PRINTTESTRESULTS("Flow", 1, (*((char *)vsa + 192) == 0));
	
	result2 = VSAAlloc(vsa, 8);
	
	PRINTTESTRESULTS("Flow", 2, (result2 == NULL));
	
	VSAFree(result);
	PRINTTESTRESULTS("Flow", 3, (*((char *)vsa + 72) == -40));
	
	result2 = VSAAlloc(vsa, 56);
	PRINTTESTRESULTS("Flow", 4, (result2 == NULL));
	
	/**/
	
	free(buffer);	
}

void TestVSALargestChunk()
{
	void *buffer = calloc(sizeof(char),100);
	vsa_t *vsa = VSAInit(buffer, 100);	
	void *result = VSAAlloc(vsa, 17);
	result = VSAAlloc(vsa, 25);
	VSAFree(result);


	PRINTTESTRESULTS("VSALargestChunk", 1, (VSALargestChunk(vsa) == 52));

	free(buffer);	
}

void TestVSAFree()
{
	void *buffer = calloc(sizeof(char),100);
	vsa_t *vsa = VSAInit(buffer, 100);	
	void *result = VSAAlloc(vsa, 17);
	result = VSAAlloc(vsa, 25);
	VSAFree(result);
	PRINTTESTRESULTS("VSAFree", 1, (*((char *)vsa + 32) == -32));

	result = VSAAlloc(vsa, 33);
	printf("%d\n",*((char *)vsa + 32));
	PRINTTESTRESULTS("VSAFree", 2, (*((char *)vsa + 32) == 52));
	
	VSAFree(result);

	PRINTTESTRESULTS("VSAFree", 3, (*((char *)vsa + 32) == -52));
	
	free(buffer);	
}

void TestVSAAlloc()
{
	void *buffer = calloc(sizeof(char),100);
	vsa_t *vsa = VSAInit(buffer, 100);	
	void *result = VSAAlloc(vsa, 17);
	/*printf("%d\n",*((char *)result - 8));*/

	PRINTTESTRESULTS("VSAAlloc", 1, ((*((char *)result - 8) == 24)));

	VSAAlloc(vsa, 25);
	PRINTTESTRESULTS("VSAAlloc", 2, (*((char *)vsa + 32) == 32));

	result = VSAAlloc(vsa, 50);
	PRINTTESTRESULTS("VSAAlloc", 3, (result == NULL));

	free(buffer);	
}

void TestVSAInit()
{
	void *buffer = malloc(100);
	vsa_t *vsa = VSAInit(buffer, 100);
	PRINTTESTRESULTS("VSAInit", 1, (*(char *)vsa == -84));
	PRINTTESTRESULTS("VSAInit", 2, (*((char *)vsa + 92) == 0));
	
	free(buffer);	
}

int main()
{
	#ifndef NDEBUG
	TestFlow();
	TestVSALargestChunk();
	TestVSAAlloc();
	TestVSAInit();
	TestVSAFree();
    #endif
    

	#ifdef NDEBUG
	TestVSAFreeDebug();
	TestVSALargestChunkDebug();
    TestFlowDebug();/**/
    #endif

/*
	*/
	return 0;
}
