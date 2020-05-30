#include <stdio.h>
#include <unistd.h>
#include "watcher.h"


int main(int argc, char *argv[])
{

	printf("starting test\n");
	printf("status is %d\n",Mmi(argv, 1, 5));
	sleep(5);
	/*KillMe();*/

return 0;
}


