#include <stdio.h> 
#include <string.h> 
#include "mem.h"

int main()
{
	char ptr[] = "123456789123456789";
	char ptr2[] = "123456789123456789";
	char c = 'z';
	size_t n = 9;
/*
	printf("%s",(ptr));
	mymemset((ptr+5), c, n); 
	printf("\n%s\n",(ptr));
	printf("%s",(ptr2));
	memset((ptr2+5), c, n); 
	printf("\n%s\n",(ptr2));

	mymemcpy(ptr, ptr2, n);

		*/

	memmove(ptr+1, ptr, n);
	mymemmove(ptr2+1, ptr2, n);
	printf("mymove%s\n", ptr2);
	printf("memove%s\n", ptr);
	
	

return (0);
}

