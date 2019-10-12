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
void esc();
void akey();
void tkey();
void Empty();
#define FIVEEmpty Empty, Empty, Empty, Empty, Empty
#define TENEmpty FIVEEmpty, FIVEEmpty
#define HUNDREDEmpty TENEmpty, TENEmpty, TENEmpty, TENEmpty, TENEmpty, \
	TENEmpty, TENEmpty, TENEmpty, TENEmpty, TENEmpty
							  			
/*only activate one function at a time */
int main ()
{
	char input = '\0';

	system("stty -icanon -echo");
	input = getchar();
	
	/*SwitchCase(input);*/
	IfElse(input);
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
	static void (*lut_arr[256])() = {TENEmpty, TENEmpty, FIVEEmpty, Empty, 
		Empty, esc, TENEmpty, TENEmpty, TENEmpty, FIVEEmpty, Empty, 
		Empty, akey, TENEmpty, FIVEEmpty, Empty, Empty, Empty, tkey,
 		HUNDREDEmpty, TENEmpty, TENEmpty, TENEmpty, TENEmpty, TENEmpty, 
		TENEmpty, TENEmpty, Empty};
	char ch = '\0';
	
	while(1)
	{
		ch = getchar();
		(*lut_arr[(unsigned int) ch])();
	}
}

void esc()
{
	system("stty icanon echo");
	exit(0);
}

void tkey()
{
	printf("T Pressed\n");
	return;
}

void akey()
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
				return;
			default:
				Empty();
		}
	}
}
