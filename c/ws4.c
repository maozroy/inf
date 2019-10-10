#include <stdio.h>
#include <stdlib.h>


void IfElse(char ch);
void SwitchCase(char ch);
void LUT(char ch);
void esc();
void akey();
void tkey();
void Empty();
void (*lut_arr[256])(void) =  {0,0,0,0,0,0,0,0,0,0,			/*0-9*/
				  			  0,0,0,0,0,0,0,0,0,0,			/*10-19*/
			  		  		  0,0,0,0,0,0,0,esc,0,0,	/*20-29*/
							  0,0,0,0,0,0,0,0,0,0,			/*30-39*/
							  0,0,0,0,0,0,0,0,0,0,			/*40-49*/
							  0,0,0,0,0,0,0,0,0,0,			/*50-59*/
							  0,0,0,0,0,akey,0,0,0,0, 	/*60-69*/	
							  0,0,0,0,0,0,0,0,0,0,			/*70-79*/
							  0,0,0,0,tkey,0,0,0,0,0,  	/*80-89*/
							  0,0,0,0,0,0,0,0,0,0,		/*90-99*/
							  0,0,0,0,0,0,0,0,0,0,			/*100-109*/
							  0,0,0,0,0,0,0,0,0,0,		/*110-119*/
							  0,0,0,0,0,0,0,0,0,0,			/*120-129*/
							  0,0,0,0,0,0,0,0,0,0,			/*130-139*/
							  0,0,0,0,0,0,0,0,0,0,			/*140-149*/
							  0,0,0,0,0,0,0,0,0,0,			/*150-159*/
							  0,0,0,0,0,0,0,0,0,0,			/*160-169*/
							  0,0,0,0,0,0,0,0,0,0,			/*170-179*/
							  0,0,0,0,0,0,0,0,0,0,			/*180-189*/
							  0,0,0,0,0,0,0,0,0,0,			/*190-199*/
							  0,0,0,0,0,0,0,0,0,0,			/*200-209*/
							  0,0,0,0,0,0,0,0,0,0,           /*210-219*/
							  0,0,0,0,0,0,0,0,0,0,           /*220-229*/
							  0,0,0,0,0,0,0,0,0,0,           /*230-239*/
							  0,0,0,0,0,0,0,0,0,0,           /*240-249*/
							  0,0,0,0,0,0 };                   /*250-255*/
							  			

int main ()
{
int input = 0;

system("stty -icanon -echo");

input = getchar();
/*SwitchCase(input);*/
LUT(input);



system("stty icanon echo");
return 0;
}

void Empty()
{
printf("...");
}

void LUT(char ch)
{
	int i = 0;
	void (*lut_arr[256]) ();

	
	while (i < 256)
	{
		lut_arr[i] = NULL;
		++i;
	}
	
	lut_arr[27] = esc;
	lut_arr[65] = akey;
	lut_arr[84] = tkey;
	
	while(1)
	{
		ch = getchar();
		lut_arr[(unsigned int)ch]();
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
}

void akey()
{
	printf("A Pressed\n");
}


void IfElse(char ch)
{
	while(1)
	{
		if(ch == 'A')
		{
			printf("%c Pressed\n", ch);
			ch = getchar();
		}
		if(ch == 'T')
		{
			printf("%c Pressed\n", ch);
			ch = getchar();
		}
		if(ch == 27)
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
				return;
		}
	}
}
