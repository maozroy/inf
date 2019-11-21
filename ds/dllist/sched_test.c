#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /*strcmp*/
#include <unistd.h>

#include "task.h"
#include "scheduler.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

void TestSchedCreate();
void TestSchedAdd();
void TestSchedRemove();
void TestSchedSize();
void TestSchedIsEmpty();
void TestSchedRun();
void TestSchedStop();
void TestSchedClear();
void TestSizeFile();
void TestRemoveFile();
void TestClearFile();
void TestDestroyFile();
void TestEmptyFile();
int PrintHelloAndReturn(void *action_func_param);
int PrintHelloAndDontReturn(void *action_func_param);
int StopFunc(void *action_func_param);
int SizeFunc(void *action_func_param);
int IsEmptyFunc(void *action_func_param);
int ClearFunc(void *action_func_param);
int CreateFunc(void *action_func_param);
int DestroyFunc(void *action_func_param);
int RemoveFunc(void *action_func_param);
void TestRemoveitself();

typedef struct remove_arguments
{
	scheduler_t *sched;
	ilrd_uid_t task;
}remove_arguments_t;



int main()
{
	TestSchedCreate();
	TestSchedAdd();
	TestSchedClear();
	TestSchedRemove();
	TestSchedRun();
	TestSchedSize();
	TestSchedIsEmpty();
	TestSchedStop();
	TestSizeFile();
	TestRemoveFile();
	TestEmptyFile();
	TestClearFile();
	TestRemoveitself();
	
	return 0;
}

int StopFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("stop", F_OK);
	if (0 == file_exist)
	{
		SchedStop((scheduler_t *)action_func_param);
	}

	return 1;
}

int SizeFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("size", F_OK);
	if (0 == file_exist)
	{
		printf("%ld\n", SchedSize((scheduler_t *)action_func_param));
	}

	return 1;
}

int IsEmptyFunc(void *action_func_param)
{
	int file_exist = 1;
	int result = 0;
	file_exist = access("empty", F_OK);
	if (0 == file_exist)
	{
		result = SchedIsEmpty((scheduler_t *)action_func_param);
		TEST1(0, result);
	}

	return 1;
}

int ClearFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("clear", F_OK);
	if (0 == file_exist)
	{
		SchedClear((scheduler_t *)action_func_param);
	}

	return 1;
}
/*
int AddFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("add", F_OK);
	if (0 == file_exist)
	{
		SchedAdd((scheduler_t *)action_func_param);
	}

	return 1;
}d
*/
int RemoveFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("remove", F_OK);
	if (0 == file_exist)
	{
		SchedRemove(((remove_arguments_t *)action_func_param)->sched, ((remove_arguments_t*)action_func_param)->task);
	}

	return 0;
}


int DestroyFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("destroy", F_OK);
	if (0 == file_exist)
	{
		SchedDestroy((scheduler_t *)action_func_param);
	}

	return 1;
}

int PrintHelloAndReturn(void *action_func_param)
{
	UNUSED(action_func_param);
	printf("hello\n");
	
	return 1;
}

int PrintHelloAndDontReturn(void *action_func_param)
{
	UNUSED(action_func_param);
	printf("hello world\n");
	
	return 0;
}

void TestSchedCreate()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSchedCreate\n");
	TEST2(scheduler);
	TEST1(SchedIsEmpty(scheduler), 1);
	TEST1(SchedSize(scheduler), 0);
	SchedDestroy(scheduler);
}

void TestSchedAdd()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSchedAdd\n");
	SchedAdd(scheduler, 20, PrintHelloAndReturn, NULL);
	TEST1(SchedIsEmpty(scheduler), 0);
	TEST1(SchedSize(scheduler), 1);
	SchedAdd(scheduler, 1, PrintHelloAndReturn, NULL);
	TEST1(SchedSize(scheduler), 2);
	SchedDestroy(scheduler);
}

void TestSchedRemove()
{
	scheduler_t *scheduler = SchedCreate();
	ilrd_uid_t uid_task1 = {0};
	ilrd_uid_t uid_task2 = {0};
	printf("TestSchedRemove\n");
	uid_task1 = SchedAdd(scheduler, 20, PrintHelloAndReturn, NULL);
	uid_task2 = SchedAdd(scheduler, 30, PrintHelloAndDontReturn, NULL);
	TEST1(SchedIsEmpty(scheduler), 0);
	TEST1(SchedSize(scheduler), 2);
	SchedRemove(scheduler,uid_task1);
	TEST1(SchedSize(scheduler), 1);
	SchedDestroy(scheduler);
}

void TestSchedRun()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSchedRun\n");
	SchedAdd(scheduler, 5, PrintHelloAndReturn, NULL);
	SchedAdd(scheduler, 10, PrintHelloAndDontReturn, NULL);
	SchedAdd(scheduler, 1, StopFunc, scheduler);
	TEST1(SchedIsEmpty(scheduler), 0);
	SchedRun(scheduler);
	SchedDestroy(scheduler);
}

void TestSchedClear()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSchedClear\n");
	SchedAdd(scheduler, 20, PrintHelloAndReturn, NULL);
	TEST1(SchedIsEmpty(scheduler), 0);
	TEST1(SchedSize(scheduler), 1);
	SchedAdd(scheduler, 30, PrintHelloAndDontReturn, NULL);
	TEST1(SchedSize(scheduler), 2);
	SchedClear(scheduler);
	TEST1(SchedSize(scheduler), 0);
	SchedDestroy(scheduler);
}

void TestSchedSize()
{
	scheduler_t *scheduler = SchedCreate();
	ilrd_uid_t uid_task1 = {0};
	ilrd_uid_t uid_task2 = {0};
	ilrd_uid_t uid_task3 = {0};
	printf("TestSchedSize\n");
	uid_task1 = SchedAdd(scheduler, 5, PrintHelloAndReturn, NULL);
	uid_task2 = SchedAdd(scheduler, 10, PrintHelloAndDontReturn, NULL);
	uid_task3 = SchedAdd(scheduler, 1, StopFunc, scheduler);
	TEST1(SchedSize(scheduler), 3);
	SchedRemove(scheduler,uid_task1);
	TEST1(SchedSize(scheduler), 2);
	SchedRemove(scheduler,uid_task3);
	TEST1(SchedSize(scheduler), 1);
	SchedRemove(scheduler,uid_task2);
	TEST1(SchedSize(scheduler), 0);
	SchedDestroy(scheduler);	
}

void TestSchedIsEmpty()
{
	scheduler_t *scheduler = SchedCreate();
	ilrd_uid_t uid_task1 = {0};
	ilrd_uid_t uid_task2 = {0};
	printf("TestSchedIsEmpty\n");
	TEST1(SchedIsEmpty(scheduler), 1);
	uid_task1 = SchedAdd(scheduler, 5, PrintHelloAndReturn, NULL);
	uid_task2 = SchedAdd(scheduler, 10, PrintHelloAndDontReturn, NULL);
	TEST1(SchedIsEmpty(scheduler), 0);
	SchedRemove(scheduler,uid_task1);
	TEST1(SchedIsEmpty(scheduler), 0);
	SchedRemove(scheduler,uid_task2);
	TEST1(SchedIsEmpty(scheduler), 1);
	SchedDestroy(scheduler);	
}

void TestSchedStop()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSchedStop\n");
	SchedAdd(scheduler, 5, PrintHelloAndReturn, NULL);
	SchedAdd(scheduler, 10, PrintHelloAndDontReturn, NULL);
	SchedAdd(scheduler, 1, StopFunc, scheduler);
	SchedRun(scheduler);
	/*stop*/
	sleep(5);
	printf("sleep now\n");
	sleep(5);
	TEST1(SchedSize(scheduler), 2);
	SchedRun(scheduler);
	SchedDestroy(scheduler);
}

void TestSizeFile()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSizeFile\n");
	SchedAdd(scheduler, 2, PrintHelloAndReturn, NULL);
	SchedAdd(scheduler, 4, PrintHelloAndDontReturn, NULL);
	SchedAdd(scheduler, 6, ClearFunc, scheduler);
	SchedAdd(scheduler, 2, SizeFunc, scheduler);
	SchedRun(scheduler);
	TEST1(SchedSize(scheduler), 0);
	SchedDestroy(scheduler);	
}

void TestRemoveFile()
{
	scheduler_t *scheduler = SchedCreate();
	remove_arguments_t *param = (remove_arguments_t *)malloc(sizeof(remove_arguments_t));
	ilrd_uid_t uid_task1 = SchedAdd(scheduler, 2, PrintHelloAndReturn, NULL);
	param->sched = scheduler;
	param->task = uid_task1;
	printf("TestRemoveFile\n");
	SchedAdd(scheduler, 2, PrintHelloAndDontReturn, NULL);
	SchedAdd(scheduler, 5, RemoveFunc, param);
	SchedRun(scheduler);
	TEST1(SchedSize(scheduler), 0);
	free(param);
	SchedDestroy(scheduler);	
}

void TestRemoveitself()
{
	scheduler_t *scheduler = SchedCreate();
	remove_arguments_t *param = (remove_arguments_t *)malloc(sizeof(remove_arguments_t));
	ilrd_uid_t uid_task1 = 	SchedAdd(scheduler, 5, RemoveFunc, param);

	printf("TestRemoveitself\n");

	param->sched = scheduler;
	param->task = uid_task1;
	SchedRun(scheduler);
	TEST1(SchedSize(scheduler), 0);
	free(param);
	SchedDestroy(scheduler);	
}

void TestClearFile()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestClearFile\n");
	SchedAdd(scheduler, 2, PrintHelloAndReturn, NULL);
	SchedAdd(scheduler, 4, PrintHelloAndDontReturn, NULL);
	SchedAdd(scheduler, 6, ClearFunc, scheduler);
	SchedRun(scheduler);
	TEST1(SchedSize(scheduler), 0);
	SchedDestroy(scheduler);	
}

void TestEmptyFile()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestEmptyFile\n");
	SchedAdd(scheduler, 2, PrintHelloAndReturn, NULL);
	SchedAdd(scheduler, 4, PrintHelloAndDontReturn, NULL);
	SchedAdd(scheduler, 4, IsEmptyFunc, scheduler);
	SchedAdd(scheduler, 10, ClearFunc, scheduler);
	SchedRun(scheduler);
	SchedDestroy(scheduler);	
}
