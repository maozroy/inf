/************************/
/*          WS6         */
/* Author: Maoz Roytman */
/* Reviwer: Eyal R.     */
/*        15/10/19      */
/************************/

#include <stdio.h> /*for prints*/

long Pow2 (unsigned int x, unsigned int y);
int IsPower2 (unsigned int n);
int AddOne (unsigned int n);
void ThreeBitsOn (unsigned int *arr, int size);
unsigned char ByteMirror(unsigned char num);
unsigned char ByteMirrorNoLoop(unsigned char num);
int TwoAndSixBothOn(unsigned int num);
int TwoAndSixOneOn(unsigned int num);
int SwapThreeFiveBits(unsigned int num);
unsigned int CheckByte(unsigned int num, int loc);
unsigned int ClosestSmallerNum(unsigned int num);
void SwapInt(int *a, int *b);
unsigned int CountWithLoop(unsigned int num);
int IsPower2NoLoop (unsigned int n);
int CountBits(unsigned int x);
void PrintInt(unsigned int num);
void printFloat(float num);
unsigned int CheckByteFloat(float num, int loc);

void Test_Pow2();
void Test_IsPower2();
void Test_IsPower2NoLoop();
void Test_AddOne();
void Test_ThreeBitsOn();
void Test_ByteMirror();
void Test_ByteMirrorNoLoop();
void Test_TwoAndSixBothOn();
void Test_TwoAndSixOneOn();
void Test_SwapThreeFiveBits();
void Test_ClosestSmallerNum();
void Test_SwapInt();
void Test_CountBits();
void Test_CountWithLoop();

int main ()
{
	float fl = 8.2;
	
	printFloat(fl);
	Test_Pow2();
	Test_IsPower2();
	Test_IsPower2NoLoop();
	Test_AddOne();
	Test_ThreeBitsOn();
	Test_ByteMirror();
	Test_ByteMirrorNoLoop();
	Test_TwoAndSixBothOn();
	Test_TwoAndSixOneOn();
	Test_SwapThreeFiveBits();
	Test_ClosestSmallerNum();
	Test_SwapInt();
	Test_CountBits();
	Test_CountWithLoop();
	
	return 0;
}

void Test_CountWithLoop()
{
	printf("\n\t\t---Test CountWithLoop---");
	if(CountWithLoop(212) == 4)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(CountWithLoop(5555) == 8)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_CountBits()
{
	printf("\n\t\t---Test CountBits---");
	if(CountBits(212) == 4)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(CountBits(5555) == 8)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_SwapInt()
{
	int a = 1;
	int aa = 1;
	int b = 2;
	int bb = 2;
	printf("\n\t\t---Test SwapInt---");
	SwapInt(&a, &b);
	if((a == bb) && (b == aa))
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
}

void Test_ClosestSmallerNum()
{
	printf("\n\t\t---Test ClosestSmallerNum---");
	if(ClosestSmallerNum(212) == 208)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(ClosestSmallerNum(50) == 48)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_SwapThreeFiveBits()
{
	printf("\n\t\t---Test SwapThreeFiveBits---");
	if(SwapThreeFiveBits(212) == 212)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(SwapThreeFiveBits(50) == 38)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_TwoAndSixOneOn()
{
	printf("\n\t\t---Test TwoAndSixOneOn---");
	if(TwoAndSixOneOn(24) == 0)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(TwoAndSixOneOn(34) == 1)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_TwoAndSixBothOn()
{
	printf("\n\t\t---Test TwoAndSixBothOn---");
	if(TwoAndSixBothOn(24) == 0)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(TwoAndSixBothOn(34) == 1)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_ByteMirror()
{
	printf("\n\t\t---Test ByteMirror---");
	if(ByteMirror(24) == 24)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(ByteMirror(1) == 128)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_ByteMirrorNoLoop()
{
	printf("\n\t\t---Test ByteMirrorNoLoop---");
	if(ByteMirrorNoLoop(24) == 24)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(ByteMirrorNoLoop(1) == 128)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_Pow2()
{
	printf("\n\t\t---Test Pow2---");
	if(Pow2(2, 3) == 16)
	{
		printf("\nTest Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
}

void Test_IsPower2()
{
	printf("\n\t\t---Test IsPower2---");
	if(IsPower2(24) == 0)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(IsPower2(16) == 1)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_IsPower2NoLoop()
{
	printf("\n\t\t---Test IsPower2NoLoop---");
	if(IsPower2NoLoop(24) == 0)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(IsPower2NoLoop(16) == 1)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}

void Test_AddOne()
{

	printf("\n\t\t---Test AddOne---");
	if(AddOne(24) == 25)
	{
		printf("\nFirst Test Passed\n");
	}
	else
	{
		printf("\ntest failed\n");
	}
	if(AddOne(9999) == 10000)
	{
		printf("\nSecond Test Passed\n");
	}
	else
	{
		printf("\nSecond Test failed\n");
	}
}


void Test_ThreeBitsOn()
{
	unsigned int arr[4] =  {100, 120, 14, 11};
	
	printf("\n\t\t---Test ThreeBitsOn---");
	ThreeBitsOn(arr, 4);
}

int CountBits (unsigned int x)
{
	unsigned int m1  = 1431655765; /*binary: 0101...*/
	unsigned int m2  = 858993459; /*binary: 00110011..*/
	unsigned int m4  = 252645135; /*binary:  4 zeros,  4 ones ...*/
	unsigned int m8  = 16711935; /*binary:  8 zeros,  8 ones ...*/
	unsigned int m16 = 65535; /*binary: 16 zeros, 16 ones ...*/

    x = (x & m1 ) + ((x >>  1) & m1 ); 
    x = (x & m2 ) + ((x >>  2) & m2 ); 
    x = (x & m4 ) + ((x >>  4) & m4 ); 
    x = (x & m8 ) + ((x >>  8) & m8 ); 
    x = (x & m16) + ((x >> 16) & m16);
    return x;
}

void printFloat (float num)
{
	unsigned int *temp = (unsigned int*) &num;
	
	printf("\nPrint float is called\n");
	PrintInt(*temp);
}

void PrintInt (unsigned int num)
{
	int i = (sizeof(int)*8)-1;
	
	printf("\nPrint int is called\n");
	printf("%d:",num);
	while (0 <= i)
	{
		printf("%d",CheckByte(num, i));
		--i;
	}
}

long Pow2 (unsigned int x, unsigned int y)
{
	return x << y;
}

int IsPower2 (unsigned int n)
{
	int size = (sizeof(int)*8);
	int counter = 0;
	
	while (size > 0)
	{
		if (CheckByte(n, size))
		{
			++counter;
		}
		--size;
	}
	if (1 < counter)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int IsPower2NoLoop (unsigned int n)
{
	return n && (!((n-1)&n));
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
	while (size_of_arr > 0)
	{
		one = 1;
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
			printf ("\n%d has 3 ones\n", *arr);
		}
		else
		{
			printf ("\n%d does not have 3 ones\n", *arr);
		}
		
		how_many_ones = 0;
		++arr;
		--size_of_arr;
	}

	return;
}

unsigned char ByteMirror (unsigned char num)
{
	unsigned int i = 0;
	unsigned char tmp = 0;
		      
	for(; i < (((sizeof(char)*8) - 1)); i++)
	{	      
		tmp = tmp | (num & 1);        
		num = num >> 1; 
		tmp = tmp << 1;        
	}
	return tmp;
}

unsigned char ByteMirrorNoLoop (unsigned char num)
{
	unsigned char maskA1 = 85; /*01010101*/
	unsigned char maskA2 = 170; /*10101010*/
	unsigned char maskB1 = 51; /*00110011*/
	unsigned char maskB2 = 204; /*11001100*/
	unsigned char maskC1 = 15; /*00001111*/
	unsigned char maskC2 = 240; /*11110000*/
	unsigned char left = 0;
	unsigned char right = 0;
	
	right = num & maskA1;
	left = num & maskA2;
	left >>=1;
	right <<=1;
	num = right | left;
	
	right = num & maskB1;
	left = num & maskB2;
	left >>=2;
	right <<=2;
	num = right | left;
	
	right = num & maskC1;
	left = num & maskC2;
	left >>=4;
	right <<=4;
	num = right | left;
	
	return num;
}

int TwoAndSixBothOn (unsigned int num)
{
	unsigned int bit_two = num;
	unsigned int bit_six = num;
	
	bit_two >>= 1;
	bit_two &= 1;
	bit_six >>=5;
	bit_six &=1;
	return (bit_six & bit_two);
}

int TwoAndSixOneOn (unsigned int num)
{
	unsigned int bit_two = num;
	unsigned int bit_six = num;
	
	bit_two >>= 1;
	bit_two &= 1;
	bit_six >>=5;
	bit_six &=1;
	return (bit_six | bit_two);
}

unsigned int CheckByte (unsigned int num, int loc)
{
	num >>=loc;
	return (1&num); 
}

int SwapThreeFiveBits (unsigned int num)
{
	unsigned int bit_three = CheckByte(num, 2);
	unsigned int bit_five = CheckByte(num, 4);
	unsigned int xor_result = 0;
	xor_result = bit_three^bit_five;
	bit_three = xor_result<<2;
	bit_five = xor_result<<4;
	xor_result = bit_three | bit_five;
	num = num^xor_result;
	
	return (num);
}

unsigned int ClosestSmallerNum (unsigned int num)
{
	num>>=4;
	num<<=4;
	return num;
}

void SwapInt(int *a, int *b)
{	
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

unsigned int CountWithLoop (unsigned int num)
{
	unsigned int i = 0;
	unsigned int counter = 0;
	
	while (i < sizeof(int)*8)
	{
		++i;
		counter += CheckByte(num, i);
	}
	return counter;
}



