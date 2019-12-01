
#include "vsa.h"
#include <stdio.h>
#include <stdlib.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 



void TESTVSAInit (void);

int main ()
{
	TESTVSAInit();
	return 0;
}

void TESTVSAInit (void)
{

	size_t magic_number = 943579852543;
	size_t memory_size = 150;
	char *pool = malloc(memory_size);
	vsa_t *my_vsa = NULL;
	my_vsa = VSAInit(pool, memory_size);
	
	#ifdef DEBUG
	PRINT_TEST(*(size_t *)(pool + 8) == magic_number, "magic_test", 1);
	#endif

}
