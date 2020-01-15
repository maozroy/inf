#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /*strcmp*/
#include <unistd.h>

#include "../../ds/task/task.h"
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
void TestIsEmptyFile();
void TestClearFile();
int TaskFunc1(void *action_func_param);
int TaskFunc2(void *action_func_param);
int StopFunc(void *action_func_param);
int SizeFunc(void *action_func_param);
int IsEmptyFunc(void *action_func_param);
int ClearFunc(void *action_func_param);
int CreateFunc(void *action_func_param);
int DestroyFunc(void *action_func_param);
int RemoveFunc(void *action_func_param);

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
		/*TestClearFile();*/
	TestSizeFile();
	TestIsEmptyFile();

	
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
	file_exist = access("empty", F_OK);
	if (0 == file_exist)
	{
		printf("%d\n",  SchedIsEmpty((scheduler_t *)action_func_param));
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

int RemoveFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("remove", F_OK);
	if (0 == file_exist)
	{	printf("remove worked");
		SchedRemove(((remove_arguments_t *)action_func_param)->sched, ((remove_arguments_t*)action_func_param)->task);
	}

	
	return 1;
}

int CreateFunc(void *action_func_param)
{
	int file_exist = 1;
	UNUSED(action_func_param);
	file_exist = access("dummy", F_OK);
	if (0 == file_exist)
	{
		SchedCreate();
	}

	return 1;
}

int DestroyFunc(void *action_func_param)
{
	int file_exist = 1;
	file_exist = access("dummy", F_OK);
	if (0 == file_exist)
	{
		SchedDestroy((scheduler_t *)action_func_param);
	}

	return 1;
}

int TaskFunc1(void *action_func_param)
{
	UNUSED(action_func_param);
	printf("hello\n");
	
	return 1;
}

int TaskFunc2(void *action_func_param)
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
	SchedAdd(scheduler, 20, TaskFunc1, NULL);
	TEST1(SchedIsEmpty(scheduler), 0);
	TEST1(SchedSize(scheduler), 1);
	SchedAdd(scheduler, 1, TaskFunc1, NULL);
	TEST1(SchedSize(scheduler), 2);
	SchedDestroy(scheduler);
}

void TestSchedRemove()
{
	scheduler_t *scheduler = SchedCreate();
	ilrd_uid_t uid_task1 = {0};
	ilrd_uid_t uid_task2 = {0};
	printf("TestSchedRemove\n");
	uid_task1 = SchedAdd(scheduler, 20, TaskFunc1, NULL);
	uid_task2 = SchedAdd(scheduler, 30, TaskFunc2, NULL);
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
	SchedAdd(scheduler, 5, TaskFunc1, NULL);
	SchedAdd(scheduler, 10, TaskFunc2, NULL);
	SchedAdd(scheduler, 1, StopFunc, scheduler);
	TEST1(SchedIsEmpty(scheduler), 0);
	SchedRun(scheduler);
	SchedDestroy(scheduler);
}

void TestSchedClear()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSchedClear\n");
	SchedAdd(scheduler, 20, TaskFunc1, NULL);
	TEST1(SchedIsEmpty(scheduler), 0);
	TEST1(SchedSize(scheduler), 1);
	SchedAdd(scheduler, 30, TaskFunc2, NULL);
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
	uid_task1 = SchedAdd(scheduler, 5, TaskFunc1, NULL);
	uid_task2 = SchedAdd(scheduler, 10, TaskFunc2, NULL);
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
	uid_task1 = SchedAdd(scheduler, 5, TaskFunc1, NULL);
	uid_task2 = SchedAdd(scheduler, 10, TaskFunc2, NULL);
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
	SchedAdd(scheduler, 5, TaskFunc1, NULL);
	SchedAdd(scheduler, 10, TaskFunc2, NULL);
	SchedAdd(scheduler, 1, StopFunc, scheduler);
	SchedRun(scheduler);
	/*stop*/
	sleep(5);
	TEST1(SchedSize(scheduler), 2);
	SchedRun(scheduler);
	SchedDestroy(scheduler);
}

void TestSizeFile()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestSizeFile\n");
	SchedAdd(scheduler, 2, TaskFunc1, NULL);
	SchedAdd(scheduler, 4, TaskFunc2, NULL);
	SchedAdd(scheduler, 6, ClearFunc, scheduler);
	SchedAdd(scheduler, 2, SizeFunc, scheduler);
	SchedRun(scheduler);
	TEST1(SchedSize(scheduler), 0);
	SchedDestroy(scheduler);	
}

void TestIsEmptyFile()
{
	scheduler_t *scheduler = SchedCreate();
	remove_arguments_t *param = {0};
	ilrd_uid_t uid_task1 = SchedAdd(scheduler, 5, TaskFunc1, NULL);
	param = (remove_arguments_t *)malloc(sizeof(remove_arguments_t));
	param->sched = scheduler;
	param->task = uid_task1;
	printf("TestIsEmptyFile\n");
	SchedAdd(scheduler, 2, TaskFunc2, NULL);
	SchedAdd(scheduler, 4, IsEmptyFunc, scheduler);
	/*SchedAdd(scheduler, 10, RemoveFunc, param);*/
	SchedAdd(scheduler, 15, ClearFunc, param);
	SchedRun(scheduler);
	TEST1(SchedSize(scheduler), 1);
	SchedDestroy(scheduler);	
}
/*
void TestClearFile()
{
	scheduler_t *scheduler = SchedCreate();
	printf("TestClearFile\n");
	SchedAdd(scheduler, 2, TaskFunc1, NULL);
	SchedAdd(scheduler, 4, TaskFunc2, NULL);
	SchedAdd(scheduler, 6, ClearFunc, scheduler);
	SchedRun(scheduler);
	TEST1(SchedSize(scheduler), 0);
	SchedDestroy(scheduler);	
}*/
