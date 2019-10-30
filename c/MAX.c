#define MAX2(a, b) ((a > b) ? a : b)
#define MAX3(a, b, c) (MAX2(a, b) < c) ? c : MAX2(a, b)
#define mysize(a) ((char*)(&a+1) - (char*)&a)
#define mysize_type(type) ((type*)0+1)

#include <stdio.h>

struct func_t
{
	char *op; /*string to activate operation*/			
	int (*comparing)(char* str, char *function); /*cmp op and usr input*/
};

int main()
{
	struct func_t bla;
	
	int a = 100;
	int b = 20;
	int c = 30;
	printf("%d is biggest", MAX3(a, b, c));
	printf("\n%d",b);
	printf("\n%d",sizeof(struct func_t));

    printf("double type size: %d\n", mysize_type(struct func_t));
	
	return (0);
}



