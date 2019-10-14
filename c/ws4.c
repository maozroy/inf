/**************************/
/*          WS4           */
/* Author: Maoz Roytman   */
/* Reviwer: Sharon Rottner*/
/*        12/10/19        */
/**************************/

#include <stdio.h> /* for prints */
#include <stdlib.h> /* for exit */

void IfElse(char ch);
void SwitchCase(char ch);
void LUT();
void Esc();
void Akey();
void Tkey();
void Empty();
#define FIVE_EMPTY Empty, Empty, Empty, Empty, Empty
#define TEN_EMPTY FIVE_EMPTY, FIVE_EMPTY
#define HUNDRED_EMPTY TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, \
	TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY
							  			
/*only activate one function at a time */
int main ()
{
	char input = '\0';

	system("stty -icanon -echo");
	input = getchar();
	SwitchCase(input);
	/*IfElse(input);*/
	/*LUT();*/
	system("stty icanon echo");
	return 0;
}

void Empty()
{
	printf("...");
	return;
}

void LUT()
{
	static void (*lut_arr[256])() = {TEN_EMPTY, TEN_EMPTY, FIVE_EMPTY, Empty, 
		Empty, Esc, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, FIVE_EMPTY, Empty, 
		Empty, Akey, TEN_EMPTY, FIVE_EMPTY, Empty, Empty, Empty, Tkey,
 		HUNDRED_EMPTY, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, TEN_EMPTY, 
		TEN_EMPTY, TEN_EMPTY, Empty};
	char ch = '\0';
	
	while(1)
	{
		ch = getchar();
		(*lut_arr[(unsigned int) ch])();
	}
}

void Esc()
{
	system("stty icanon echo");
	exit(0);
}

void Tkey()
{
	printf("T Pressed\n");
	return;
}

void Akey()
{
	printf("A Pressed\n");
	return;
}


void IfElse(char ch)
{
	system("stty -icanon -echo");
	while(1)
	{
		ch = getchar();
		if('A' == ch)
		{
			printf("%c Pressed\n", ch);
		}
		if('T' == ch)
		{
			printf("%c Pressed\n", ch);
		}
		if(27 == ch)
		{
			system("stty icanon echo");
			return;
		}
	}
}


void SwitchCase(char ch)
{
	while(1)
	{
		ch = getchar();
		switch(ch)
		{
			case 'T':
				printf("%c Pressed\n", ch);
				break;
			case 'A':
				printf("%c Pressed\n", ch);
				break;
			case 27:
				system("stty icanon echo");
				exit(0);
			default:
				Empty();
				break;
		}
	}
}
