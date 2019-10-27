#include <stdio.h> 
#include <string.h> 
#include "mem.h"

int main()
{
	char ptr1[] = "0123456789ABCabc";
	char ptr2[] = "0123456789ABCabc";
	char c = 'z';
	size_t n = 13;
/*
	printf("%s",(ptr));
	mymemset((ptr+5), c, n); 
	printf("\n%s\n",(ptr));
	printf("%s",(ptr2));
	memset((ptr2+5), c, n); 
	printf("\n%s\n",(ptr2));

	mymemcpy(ptr, ptr2, n);

		*/

	memmove(ptr1+1, ptr1, n);
	mymemmove(ptr2+1, ptr2, n);
	printf("mymove%s\n", ptr2);
	printf("memove%s\n", ptr1);
	printf("\nresult is %d",strcmp(ptr1,ptr2));
	
	
	

return (0);
}

