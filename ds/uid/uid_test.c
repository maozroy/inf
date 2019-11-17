
#include "uid.h"
#include <stdio.h>
#include <unistd.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) \

void TestUIDCreate(void);
void TestUIDIsSame(void);
void TestUIDIsBad(void);

int main()
{
	TestUIDCreate();
	TestUIDIsSame();
	TestUIDIsBad();
	return 0;
}

void TestUIDCreate(void)
{
	ilrd_uid_t my_uid1 = UIDCreate();
	ilrd_uid_t my_uid2 = {0};
	
	sleep(1);
	my_uid2 = UIDCreate();
	PRINT_TEST(my_uid1.counter == 0, "counter test", 1);
	PRINT_TEST((my_uid1.pid == my_uid2.pid), "pid test", 2);
	PRINT_TEST((my_uid1.time != my_uid2.time), "time test", 3);
	PRINT_TEST(my_uid2.counter == 1, "counter test", 4);
	PRINT_TEST(my_uid2.pid == getpid(), "pid test", 5);
}

void TestUIDIsSame(void)
{
		ilrd_uid_t my_uid1 = UIDCreate();
		ilrd_uid_t my_uid2 = UIDCreate();
		ilrd_uid_t my_uid3 = UIDCreate();
		ilrd_uid_t my_uid4 = UIDCreate();
		
		my_uid2 = UIDCreate();
		PRINT_TEST(UIDIsSame(my_uid1, my_uid2) == 0, "is same test", 1);
		my_uid3.counter = my_uid4.counter;
		PRINT_TEST(UIDIsSame(my_uid3, my_uid4) == 1, "is same test", 1);
}

void TestUIDIsBad(void)
{
		ilrd_uid_t my_uid1 = UIDCreate();
		ilrd_uid_t my_uid2 = UIDCreate();
		
		my_uid2.time = -1;
		PRINT_TEST(UIDIsBad(my_uid2) == 1, "is bad test", 1);
		PRINT_TEST(UIDIsBad(my_uid1) == 0, "is bad test", 2);
}
