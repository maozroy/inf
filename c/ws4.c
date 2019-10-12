#include <stdio.h>
#include <stdlib.h>


void IfElse(char ch);
void SwitchCase(char ch);
void LUT();
void esc();
void akey();
void tkey();
void Empty();
void (*lut_arr[256])(void) =  	{Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*0-9*/
	  			 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*10-19*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, esc,Empty, Empty, 		/*20-29*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*30-39*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*40-49*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*50-59*/
				 Empty, Empty, Empty, Empty, Empty, akey,Empty, Empty, Empty, Empty,  	/*60-69*/	
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*70-79*/
				 Empty, Empty, Empty, Empty, tkey,Empty, Empty, Empty, Empty, Empty,   	/*80-89*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*90-99*/
			 	 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*100-109*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*110-119*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*120-129*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*130-139*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*140-149*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*150-159*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*160-169*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*170-179*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*180-189*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*190-199*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 		/*200-209*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,             /*210-219*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,             /*220-229*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,             /*230-239*/
				 Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,             /*240-249*/
				 Empty, Empty, Empty, Empty, Empty, 0};                  /*250-255*/
							  			
/*only activate one function at a time */
int main ()
{
	char input = 0;
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
	int ch = 0;
	
	while(1)
	{
		ch = getchar();
		(*lut_arr[(int) ch])();
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
		if(ch == 'A')
		{
			printf("%c Pressed\n", ch);
		}
		if(ch == 'T')
		{
			printf("%c Pressed\n", ch);
		}
		if(ch == 27)
		{
			system("stty icanon echo");
			return;
		}
		else
		{
			Empty();
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
				return;
			default:
				Empty();
		}
	}
}
