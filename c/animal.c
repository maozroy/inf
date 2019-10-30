#include <stdio.h>
#define offset(s,f) (size_t)( & ( (s*) 0 ) -> f )



int main()
{

	typedef struct animal
	{
		int size;
		char name[4];
		int age;
	}anim;
	int a = offset(anim, age);
	printf("%d",a);
	
	return 0;
}
