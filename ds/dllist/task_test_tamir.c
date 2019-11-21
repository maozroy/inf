/****************************************************************************
*                                                                           *
*                                Tamir Dayan                                *
*                              OL79 - 20/11/19                              *
*                                                                           *	
*                              Task Test                               *
*                           Reviewer: Chen                                  *
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */
#include <unistd.h> /* sleep */

#include "task.h" /* task */
#include "uid.h" /* Unique ID */
#include "scheduler.h" /* scheduler */
#include "myutils.h"

struct task
{
	ilrd_uid_t task_id;
	time_t time_to_run;
	time_t interval;
	task_func action_func;
	void *action_func_param;
};

void TestTaskCreate();
void TestTaskRemove();
void TestTaskGetID();
void TestTaskUpdateTimeToRun();
void TestTaskGetTimeToRun();
void TestTaskIsMatchByID();
void TestTaskRun();

static int PrintHelloImp(void *action_func_param);
static int PrintLadyGagaImp(void *action_func_param);
/*
int main()
{
	TestTaskCreate();
	TestTaskRemove();
	TestTaskGetID();
	TestTaskUpdateTimeToRun();
	TestTaskGetTimeToRun();
	TestTaskIsMatchByID();
	TestTaskRun();

	return 0;
}*/

void TestTaskCreate()
{
	task_t *task = NULL;
	time_t interval = 0;
	task_func action_func = PrintHelloImp;
	void* action_func_param = NULL;

	printf("\n*****" BBLU "TestTaskCreate" KNRM "*****\n");
	TEST("TestTaskCreate", 1, NULL == task);
	task = TaskCreate(interval, action_func, action_func_param);
	TEST("TestTaskCreate", 2, NULL != task);
	printf("***** ********* *****\n");
	TaskRemove(task);
}

void TestTaskRemove()
{
	task_t *task = NULL;
	time_t interval = 0;
	task_func action_func = PrintHelloImp;
	void* action_func_param = NULL;

	task = TaskCreate(interval, action_func, action_func_param);
	printf("\n*****" BBLU "TestTaskRemove" KNRM "*****\n");
	TEST("TestTaskRemove", 1, NULL != task);
	TaskRemove(task);
	printf(BRED "***** CHECK FOR VALGRIND *****" KNRM "\n");	
	printf("***** ********* *****\n");
}

void TestTaskGetID()
{
	task_t *task = NULL;
	time_t interval = 0;
	task_func action_func = PrintHelloImp;
	void* action_func_param = NULL;

	task = TaskCreate(interval, action_func, action_func_param);
	printf("\n*****" BBLU "TestTaskGetID" KNRM "*****\n");
	TEST("TestTaskGetID", 1, 1 == UIDIsSame(TaskGetID(task),task->task_id));
	printf(BRED "***** CHECK FOR VALGRIND *****" KNRM "\n");
	printf("***** ********* *****\n");
	TaskRemove(task);
}

void TestTaskUpdateTimeToRun()
{
	task_t *task = NULL;
	time_t time_to_run = 0;
	time_t time_to_run2 = 0;
	time_t interval = 0;
	task_func action_func = PrintHelloImp;
	void* action_func_param = NULL;

	task = TaskCreate(interval, action_func, action_func_param);
	time_to_run = TaskGetTimeToRun(task);
	TaskUpdateTimeToRun(task);
	
	printf("\n*****" BBLU "TestTaskUpdateTimeToRun" KNRM "*****\n");
	time_to_run2 = TaskGetTimeToRun(task);
	TEST("TestTaskUpdateTimeToRun", 1, 0 == difftime(time_to_run2, time_to_run));
	
	sleep(5);
	interval = 0;
	TaskUpdateTimeToRun(task);
	TEST("TestTaskUpdateTimeToRun", 2, 0 != difftime(TaskGetTimeToRun(task), 
											time_to_run2));
	printf("***** ********* *****\n");
	TaskRemove(task);
}

void TestTaskGetTimeToRun()
{
	task_t *task = NULL;
	time_t interval = 0;
	task_func action_func = PrintHelloImp;
	void* action_func_param = NULL;

	task = TaskCreate(interval, action_func, action_func_param);

	printf("\n*****" BBLU "TestTaskGetTimeToRun" KNRM "*****\n");
	TEST("TestTaskGetTimeToRun", 1, 0 == difftime(task->time_to_run, 
										 TaskGetTimeToRun(task)));
	sleep(5);
	interval = 2;
	TaskUpdateTimeToRun(task);
	TEST("TestTaskGetTimeToRun", 2, 0 == difftime(task->time_to_run, 
										 TaskGetTimeToRun(task)));
	printf("***** ********* *****\n");
	TaskRemove(task);
}

void TestTaskIsMatchByID()
{
	task_id_t id1 = {0};
	task_id_t id2 = {0};
	task_t *task = NULL;
	task_t *task2 = NULL;
	time_t interval = 0;
	task_func action_func = PrintHelloImp;
	void* action_func_param = NULL;

	task = TaskCreate(interval, action_func, action_func_param);
	id1 = TaskGetID(task);
	
	sleep(5);
	interval = 2;
	task2 = TaskCreate(interval, action_func, action_func_param);
	id2 = TaskGetID(task2);

	printf("\n*****" BBLU "TestTaskIsMatchByID" KNRM "*****\n");
	TEST("TestTaskIsMatchByID", 1, 1 == TaskIsMatchByID(id1, task));
	TEST("TestTaskIsMatchByID", 2, 0 == TaskIsMatchByID(id2, task));
	TEST("TestTaskIsMatchByID", 3, 1 == TaskIsMatchByID(id2, task2));
	TEST("TestTaskIsMatchByID", 4, 0 == TaskIsMatchByID(id1, task2));
	printf("***** ********* *****\n");
	TaskRemove(task);
	TaskRemove(task2);
	
}

void TestTaskRun()
{
	task_t *task = NULL;
	task_t *task2 = NULL;
	time_t interval = 0;
	task_func action_func = PrintHelloImp;
	void* action_func_param = NULL;

	task = TaskCreate(interval, action_func, action_func_param);
	
	sleep(5);
	interval = 2;
	action_func = PrintLadyGagaImp;
	task2 = TaskCreate(interval, action_func, action_func_param);

	printf("\n*****" BBLU "TestTaskRun" KNRM "*****\n");
	TEST("TestTaskRun", 1, 0 == TaskRun(task));
	TEST("TestTaskRun", 1, 2 == TaskRun(task2));
	printf("***** ********* *****\n");
	TaskRemove(task);
	TaskRemove(task2);
}

static int PrintHelloImp(void *action_func_param)
{
	printf("Hello! I'm Lady Gaga!\n");
	UNUSE(action_func_param);
	
	return 0;
}

static int PrintLadyGagaImp(void *action_func_param)
{
	static int counter = 0;
	UNUSE(action_func_param);
	
	printf("███████████▓▒▓▒──▒▒▒▒▓╫─░┼░░▓█▓▓▓─██████████\n");
	printf("███████████▓▓╫─────░░┼──────░▓▓▓▓▒─█████████\n");
	printf("████████████▒──░──┼┼─▒──────┼▓▓▓▓█╫─████████\n");
	printf("███████████▓──────░──░░──────▓▓▓▓▓█──███████\n");
	printf("███████████┼──────────░──────▒█▓▓▓█▓──▒█████\n");
	printf("██████████▒───────────┼───────█▓▓▓█▓▒──┼████\n");
	printf("██████████░───────────░───────▓█▓▓▓█▓▒┼──███\n");
	printf("█████████▓░───────────░───░───┼▓██▓▓▓▓▒┼──██\n");
	printf("█████████▒────────────░┼───────▓▓█▓█▓▓▓┼──▓█\n");
	printf("█████████╫─────────░──░┼░─░───░▓██▓▓▓█╫────█\n");
	printf("████████▓┼────────────░┼┼░┼┼░──▓██▓▓▓▓───░─█\n");
	printf("████████▓─────────────░┼╫┼┼┼░──▓█████▒──░┼─▓\n");
	printf("████████▒─────────────░╫╫┼┼┼┼──▓█████╫─░░╫░▓\n");
	printf("████████▒───────────░─┼╫╫┼╫┼╫──▓███▓▒──░╫╫╫▓\n");
	printf("████████╫────────░──░░░╫╫┼╫╫╫┼─▓██▒───╫▓▒▒┼▓\n");
	printf("████████╫────░┼─░░░─┼╫╫╫▒╫▒▒╫┼░▓█────╫▓▓╫┼╫▓\n");
	printf("███████▓┼──┼─┼░─░╫┼░╫▒▒▒▒╫▒▒╫╫┼▓▒▓─▓▓█▓──╫╫▓\n");
	printf("███████▓╫──╫┼░╫░╫╫╫░▒╫▒▒▓▒▒▒▒╫╫▓▓▓████──┼▒╫█\n");
	printf("███████▓▒┼─╫╫┼╫┼┼╫▒╫▒▒▒▒▒▒╫▒▒╫╫██████──┼▓▒╫█\n");
	printf("███████▓▒╫─╫▒╫╫┼┼╫▒╫▒▓▒▓▒▒╫▒▒╫▒█████───▓▓┼▒█\n");
	printf("███████▓▓▒─╫╫▒╫╫┼╫▒▒╫▓▒▓▒▓╫▒▒╫▒███▓──╫▓█░┼▒█\n");
	printf("███████▓▓▓░╫▒▒▒▓▒╫▒▒▒▓▓▓▓▓▓▒▒┼▒███──▒▒█░░▒▓█\n");
	printf("████████▓▒▓▒╫╫▒╫▓███████▓▓██▓┼▒██──┼▒█▒─▒▓██\n");
	printf("███████▒┼╫███▒──▓████████▓▓██░▓█──░▒██──█▒██\n");
	printf("██████░─╫▓████╫─▒▓████████▓▒▓▒▓╫─▒███──▒▓▒██\n");
	printf("█████──┼┼██▒▓█▒─▒▒█▒╫██████▓▓▒─▒▓███░─╫▓▒▓██\n");
	printf("████▒────███▓▓╫─┼░▒████▓▓███▓┼▒████▓─┼▓▓╫███\n");
	printf("███▓────▓██▓▓▒░─┼┼░▒█▓▓▒▒▓█▓▓░▓███▒─╫▒█─▓███\n");
	printf("███┼───████▓─╫──░┼░┼╫▒╫▒╫▒█▓▓╫▓██▓──▓█▓─████\n");
	printf("██▓─░──██▓▓▓░╫──┼░┼┼╫▒▒▒▒▒█▒▓┼▓█▒─▒▒██┼▒████\n");
	printf("██▓────██▒▒▓╫╫──┼╫░░╫▒▒▒▒▓█▒▓┼▓▓─▒▓██▓░█████\n");
	printf("██▒─░──██▒──▒╫──┼╫╫░▒▒▒▒▒▓█▒▓┼▓██████─▒█████\n");
	printf("██░─┼──██▒──▒▒───╫▒╫╫▒▒▒▓█▓╫▓╫▓█████▒▒██████\n");
	printf("██──░──██▓──▒▒────╫▒▒▒▒▒▓█▒▒▒▒██▓██▓╫██▓████\n");
	printf("██──░──██▓┼─┼▒────╫▒▒▒▒▒▓█░▓▒▓█▓▓██▒▓█▓▓████\n");
	printf("██──░──██▓▓──▓▒▒██▓┼▓▒▒▒▓▓╫▓▒▓█▓▓█▓███▓▓████\n");
	printf("██─┼░──▒█▓▓──▓███▒─╫▒▒▒▓▓╫▒▓▒█▓▓▓█▓██▓▓▓████\n");
	printf("██─┼▒───██▓──▒██╫──┼▒▒▓▓▓╫▓▓▓█▓▓▓███▓▓▓▓████\n");
	printf("██─░▒┼───▒╫──▒┼────┼▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓████\n");
	printf("██▒░╫┼───────▒▓▒▓▓╫─╫▒▓▓▒▓▓▓█▓▓█▓▓▓▓▓▓▓▓████\n");
	printf("██▒┼┼░─────░─▓███████▓▓▒▓▓▓▓███▓██▓▒▓▓▓▓████\n");
	printf("█▓█▒┼░░─░─░░┼▓█▓███▓▓▓▓▒▓▓▓███▓███▓▓▓▓▓▓████\n");
	printf("█▓█▓┼╫░░───░░██▒▒▒▒▓▓▓▓▓▓▓▓█▓▓███▓▒▓▓▓▓▓████\n");
	printf("█▓█▓╫╫┼░░░─░░▓██▓▓███▓▓▓███████▓▒▒▓▓▓▓▓██▓██\n");
	printf("█▓██┼╫┼┼┼┼░┼░██████▓▓▓▓█▓▓▓▓██▓▓▓█▓▓▓▓▓▓▒███\n");
	printf("█▒▓█▒┼╫┼┼┼┼┼┼███▓╫┼░▓█▓▒─────┼▒▓███████▓▓███\n");
	printf("█▓▓█▓┼╫╫░░┼┼▒███▓──▒█▓┼░┼░──░░░─▓███████████\n");
	printf("█▓▓█▓┼╫╫░░░─▓████▓▒█▓╫╫╫╫░░─░░┼┼─╫▓█████████\n");
	printf("██▒▓▒▒┼╫░░──████▓██▓▒▒▒▒┼┼───┼┼╫╫╫▒▓████████\n");
	printf("██▓▓░▒╫┼░───██▓▓▓▓▓▓▒▒▒▒╫┼░░░░┼┼╫╫╫▒▓███████\n");
	printf("██▓▓─▒╫┼───░██▓▓▒▓▒▒▒▒▒▒╫╫╫┼░┼┼┼╫╫▒▒▓▓██████\n");
	printf("██▓▒░▒╫┼───▒█▒▓██▒▓▒▒▒▒▒▒▒▒╫▒╫╫╫▒╫▒╫▓▓▓█████\n");
	printf("███▒░▒╫╫───▓█▓▓█▓▒▓▓▒▒▒▒▒▒▒▒▒▒╫╫▒▒▒▒▒▓▓█████\n");
	printf("███▓░▒▒┼───██▓█▓╫▒▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓██████\n");
	printf("███▓░▒▒░───█▓██░▒▓▓▓▓▓▓▓▒▓▒▒▒▒▒▒▒▒▒▒▒▓██████\n");
	printf("█▓█▓▒▒╫───▒███─▒▓▓▓▓▓▓▓▓▓▒▒▒▓▒▒▒▒▒▒▒▓▓██████\n");
	printf("█▓▓█▒╫┼───▓██╫▒▓▒▓▓▓▓▓▓▓▓▓▒▒▒▓▒▒▒▒▒▒▒▓▓█████\n");
	printf("█▓▓▓▓╫╫───██▓╫▓▒▒▓▓▓▓▓▓▓▓▒▓▒▓▓▓▒▒▒▒▒▒▓▓█████\n");
	printf("█▓▓█▓╫┼──░█▓▒▓▓▒▒▓▓▓▓▓▓▓▓▓▒▓▒▓▓▓▒▒▒▒▒▓▓█████\n");
	printf("▓▓▓█▒┼┼──┼▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▒▓▒▒▒▓▓▓█████\n\n");

	++counter;
	
	if (2 == counter)
	{
		return 0;
	}
	
	return 1;
}
