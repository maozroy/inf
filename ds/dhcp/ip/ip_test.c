
#include "ip.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

void TestIPtoBit (void);

int main()
{
TestIPtoBit();

return 0;
}

void TestIPtoBit (void)
{
	IPAddress adrs = {192, 168, 0, 1};
	BitToIp(IPtoBit(adrs), adrs);
	printf("%u",IPtoBit(adrs));
}
