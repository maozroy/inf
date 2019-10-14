#include <stdio.h>

long Pow2 (unsigned int x, unsigned int y);
int IsPower2 (unsigned int n);
int AddOne (unsigned int n);
void ThreeBitsOn (unsigned int *arr, int size);
unsigned int ByteMirror(unsigned int num);

int main ()
{
unsigned int test = 1025;
int i = 7;
unsigned int arr[4] =  {100, 120, 14, 11};

printf("test number %ld\n",Pow2(test, i));
printf("is %d power of 2? %d\n",test, IsPower2(test));
printf("%d plus 1 is? %d\n",test, AddOne(test));
printf(" 1 reversed is? %d\n",ByteMirror(1000));
ThreeBitsOn (arr, 4);

return 0;
}

long Pow2(unsigned int x, unsigned int y)
{
	return x << y;
}

int IsPower2 (unsigned int n)
{
	int i = sizeof(int)-1;
	unsigned int result = n;

	for( ;( (result & 1) != 1) &&( i > 0 ); i--)
	{
		result = n << i;
	}
	
	if (0 == i)
	{
		return (1);
	}
	else
	{
		return (0);
	}

}

int AddOne (unsigned int n)
{
	int x = 1;
	
	while (x & n )
	{
		n = x^n;
		x = x<<1;
	}
	n = n^x;
	return n;
}

void ThreeBitsOn (unsigned int *arr, int size_of_arr)
{
	int how_many_ones = 0;
	size_t i = 0;
	int one = 1;
	while ((size_of_arr-1) > 0)
	{
		one = 1;
		++arr;
		for(i = 0 ; i < sizeof(arr); i++)
		{
			if (one & *arr)
			{
				++how_many_ones;

			}
			one = one << 1;

		}
		if(3 == how_many_ones)
		{
			printf ("%d has 3 ones\n", *arr);
		}
		how_many_ones = 0;
		--size_of_arr;
	}

	return;
}




unsigned int ByteMirror(unsigned int num)
{
	unsigned int i = 0;
	unsigned int tmp = 0;
		      
	for(; i < ((sizeof(int)*8 - 1)); i++)
	{	      
		tmp = tmp | (num & 1);        
		num = num >> 1; 
		tmp = tmp << 1;        
	}
	   
	return tmp;
}
