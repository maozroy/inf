/****************************************************************************
*                                                                           *
*                                Tamir Dayan                                *
*                              OL79 - 20/11/19                              *
*                                                                           *	
*                              Scheduler Test                               *
*                           Reviewer: Chen                                  *
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "uid.h" /* Unique ID */
#include "scheduler.h" /* scheduler */
#include "myutils.h"

void TestSchedCreate();
void TestSchedDestroy();
void TestSchedAdd();
void TestSchedRemove();
void TestSchedSize();
void TestSchedIsEmpty();
void TestSchedRun();
void TestSchedStop();
void TestSchedClear();

static int StopImp(void *action_func_param);
static int PrintHelloImp(void *action_func_param);
static int PrintLadyGagaImp(void *action_func_param);

int main()
{
	TestSchedCreate();
	TestSchedDestroy();
	TestSchedAdd();
	TestSchedRemove();
	TestSchedSize();
	TestSchedIsEmpty();
	TestSchedRun();
	TestSchedStop();
	TestSchedClear();

	return 0;
}

void TestSchedCreate()
{
	scheduler_t *scheduler = NULL;

	printf("\n*****" BBLU "TestSchedCreate" KNRM "*****\n");
	TEST("TestSchedCreate", 1, NULL == scheduler);
	scheduler = SchedCreate();
	TEST("TestSchedCreate", 2, NULL != scheduler);
	TEST("TestSchedCreate", 3, 1 == SchedIsEmpty(scheduler));
	TEST("TestSchedCreate", 4, 0 == SchedSize(scheduler));
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedDestroy()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;
	ilrd_uid_t id = {0};
	
	scheduler = SchedCreate();
	
	printf("\n*****" BBLU "TestSchedDestroy" KNRM "*****\n");
	id = SchedAdd(scheduler, interval, action, action_func_param);
	interval = 2;
	action = PrintLadyGagaImp;
	SchedAdd(scheduler, interval, action, action_func_param);
	SchedRemove(scheduler, id);
	printf(BRED "***** CHECK FOR VALGRIND *****" KNRM "\n");
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedAdd()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;
	
	scheduler = SchedCreate();
	
	printf("\n*****" BBLU "TestSchedAdd" KNRM "*****\n");
	SchedAdd(scheduler, interval, action, action_func_param);
	TEST("TestSchedAdd", 1, NULL != scheduler);
	TEST("TestSchedAdd", 2, 0 == SchedIsEmpty(scheduler));
	TEST("TestSchedAdd", 3, 1 == SchedSize(scheduler));
	
	interval = 2;
	action = PrintLadyGagaImp;
	
	SchedAdd(scheduler, interval, action, action_func_param);
	TEST("TestSchedAdd", 4, NULL != scheduler);
	TEST("TestSchedAdd", 5, 0 == SchedIsEmpty(scheduler));
	TEST("TestSchedAdd", 6, 2 == SchedSize(scheduler));
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedRemove()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;
	ilrd_uid_t id1 = {0};
	ilrd_uid_t id2 = {0};
	
	scheduler = SchedCreate();
	
	printf("\n*****" BBLU "TestSchedRemove" KNRM "*****\n");
	id1 = SchedAdd(scheduler, interval, action, action_func_param);
	interval = 2;
	action = PrintLadyGagaImp;	
	id2 = SchedAdd(scheduler, interval, action, action_func_param);
	TEST("TestSchedRemove", 1, 0 == SchedRemove(scheduler, id2));
	TEST("TestSchedRemove", 2, 1 == SchedRemove(scheduler, id2));
	TEST("TestSchedRemove", 3, 0 == SchedRemove(scheduler, id1));
	TEST("TestSchedRemove", 4, 1 == SchedRemove(scheduler, id1));
	TEST("TestSchedRemove", 5, 1 == SchedRemove(scheduler, id2));
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedSize()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;
	ilrd_uid_t id1 = {0};
	ilrd_uid_t id2 = {0};
	
	scheduler = SchedCreate();
	
	printf("\n*****" BBLU "TestSchedSize" KNRM "*****\n");
	TEST("TestSchedSize", 1, 0 == SchedSize(scheduler));
	id1 = SchedAdd(scheduler, interval, action, action_func_param);
	TEST("TestSchedSize", 2, 1 == SchedSize(scheduler));
	interval = 2;
	action = PrintLadyGagaImp;	
	id2 = SchedAdd(scheduler, interval, action, action_func_param);
	TEST("TestSchedSize", 3, 2 == SchedSize(scheduler));
	SchedRemove(scheduler, id2);
	TEST("TestSchedSize", 4, 1 == SchedSize(scheduler));	
	SchedRemove(scheduler, id1);
	TEST("TestSchedSize", 5, 0 == SchedSize(scheduler));	
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedIsEmpty()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;
	ilrd_uid_t id1 = {0};
	ilrd_uid_t id2 = {0};
	
	scheduler = SchedCreate();
	
	printf("\n*****" BBLU "TestSchedIsEmpty" KNRM "*****\n");
	TEST("TestSchedIsEmpty", 1, 1 == SchedIsEmpty(scheduler));
	id1 = SchedAdd(scheduler, interval, action, action_func_param);
	TEST("TestSchedIsEmpty", 2, 0 == SchedIsEmpty(scheduler));
	interval = 2;
	action = PrintLadyGagaImp;	
	id2 = SchedAdd(scheduler, interval, action, action_func_param);
	TEST("TestSchedIsEmpty", 3, 0 == SchedIsEmpty(scheduler));
	SchedRemove(scheduler, id2);
	TEST("TestSchedIsEmpty", 4, 0 == SchedIsEmpty(scheduler));	
	SchedRemove(scheduler, id1);
	TEST("TestSchedIsEmpty", 5, 1 == SchedIsEmpty(scheduler));	
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedRun()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;
	
	scheduler = SchedCreate();
	SchedAdd(scheduler, interval, action, action_func_param);
	interval = 2;
	action = PrintLadyGagaImp;	
	SchedAdd(scheduler, interval, action, action_func_param);
	
	printf("\n*****" BBLU "TestSchedRun" KNRM "*****\n");
	TEST("TestSchedRun", 1, 2 == SchedRun(scheduler));
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedStop()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;

	scheduler = SchedCreate();
	SchedAdd(scheduler, interval, action, action_func_param);
	
	SchedAdd(scheduler, interval, StopImp, scheduler);
	
	interval = 2;
	action = PrintLadyGagaImp;	
	SchedAdd(scheduler, interval, action, action_func_param);
	
	printf("\n*****" BBLU "TestSchedStop" KNRM "*****\n");
	TEST("TestSchedStop", 1, 0 == SchedRun(scheduler));
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

void TestSchedClear()
{
	scheduler_t *scheduler = NULL;
	time_t interval = {0};
	action_func action = PrintHelloImp;
	void* action_func_param = NULL;

	scheduler = SchedCreate();
	SchedAdd(scheduler, interval, action, action_func_param);
	interval = 2;
	action = PrintLadyGagaImp;	
	SchedAdd(scheduler, interval, action, action_func_param);
	
	SchedClear(scheduler);
	printf("\n*****" BBLU "TestSchedClear" KNRM "*****\n");
	TEST("TestSchedClear", 1, 2 == SchedRun(scheduler));
	TEST("TestSchedClear", 2, 1 == SchedIsEmpty(scheduler));
	TEST("TestSchedClear", 3, 0 == SchedSize(scheduler));
	printf("***** ********* *****\n");
	SchedDestroy(scheduler);
}

static int StopImp(void *action_func_param)
{
	SchedStop((scheduler_t*)action_func_param);
	
	return 0;
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
