#include <stdio.h> /*for printf*/
#include <string.h> /*for strcpy*/
#include <stdlib.h> /*for calloc, free*/
#include <ctype.h> /*for tolower*/

void freeback(char **ptr); /*memory freeing of ** */
void printArray(char **arr); /*printing strings within ** */
void copyArray(char **arr); /*copis a ** pointer to a new temp ** pointer  */

void joseph(int n); /*resolves josephus problem*/
void printDataTypes();

int main (int argc, char* argv	[], char *env[]) 
{

int suiciders = 0;

printf("Please enter number of suiciders:");
scanf("%d", &suiciders);
joseph(suiciders);

printDataTypes();

copyArray(env);	

return 0;

}

void printDataTypes()
{
char ch = 0;
signed char si_ch = 0;

int in = 0;
unsigned int un_in = 0;

signed short int si_sh_in = 0;
signed long int si_lo_in = 0;

unsigned short int un_sh_in = 0;
unsigned long int un_lo_in = 0;



float fl = 0;
long lo_fl = 0;
double do_fl = 0;
long double lo_do_fl = 0;
int * ptr = NULL;





puts("\t\tCHARS");
printf("size of (unsigned) char    = %lu bytes\n", sizeof(ch));
printf("size of signed char        = %lu bytes\n", sizeof(si_ch));

puts("\n\t\tINTS");
printf("size of (signed) short int = %lu bytes\n", sizeof(si_sh_in));
printf("size of unsigned short int = %lu bytes\n", sizeof(un_sh_in));
printf("          -----                    \n");
printf("size of (signed) int       = %lu bytes\n", sizeof(in));
printf("size of unsigned int       = %lu bytes\n", sizeof(un_in));
printf("          -----                    \n");
printf("size of (signed) long int  = %lu bytes\n", sizeof(si_lo_in));
printf("size of unsigned long int  = %lu bytes\n", sizeof(un_lo_in));


puts("\n\t\tFLOATS");
printf("size of float              = %lu bytes\n", sizeof(fl));
printf("size of long float         = %lu bytes\n", sizeof(lo_fl));
printf("size of double float       = %lu bytes\n", sizeof(do_fl));
printf("size of long double float  = %lu bytes\n", sizeof(lo_do_fl));
puts("\n\t\tPOINTER");
printf("size of pointer            = %lu bytes\n", sizeof(ptr));


return;

}


void printArray(char **arr)
{
	char **runarr = 0;
	runarr = arr;


	while (*runarr)
	{
		printf("%s \n", *runarr);
		runarr++;
	}
	arr = runarr;
}

void copyArray(char **arr)
{
	int a = 0;
	int counter = 0;
	int i = 0;
	int blocker = 0;
	
	char **runarr = arr;
	char **temp = NULL;
	char **runtemp;
	char **printemp;
	
	while(*arr)
	{
		counter++;	
		arr++;
	}
	arr = runarr;
	
	temp = calloc(counter+1, (counter+1) * (sizeof(char*)));
	runtemp = temp;
	printemp = temp;

		while (*arr)
		{
			a = strlen(*arr);
			*temp = calloc(a+1, (a+1) * (sizeof(char) ) );
			++blocker;
			
			if(blocker > 1000)
			{	
				*temp = NULL;
				freeback(runtemp);
				return;
			}
			
			strcpy(*temp, *arr);
			
			for (i = 0 ; i < (a+1) ; i++)
			{
				*(*temp+i) = tolower(*(*temp+i));
			}
			
			temp++;
			arr++;
		}
	arr = runarr;
	temp = runtemp;
	
	printArray(printemp);
	freeback(runtemp);
}

void freeback(char **ptr)
{
	int a = 0;
	char **temp_ptr;
	int i = 0;
	
	temp_ptr = ptr;
	
	while(*ptr)
	{
		a++;	
		ptr++;
	}
	
	ptr =temp_ptr;
	
	for(i = 0; (i) < a ; i++)
	{
		free(*temp_ptr);
		temp_ptr++;
		
	}
	
	free(ptr);
	
	return;
}

void joseph(int n)
{
	int *army;
	int i = 0;
	int temp = 0;
	army = malloc( sizeof(int) * n + 1);
	
	for (i = 0 ; i < n ; i++)
	{
		army[i] = i+1;
	}
	army[n] = 0;
	i = 0;
	
	while (i != army[i])
	{
		temp = army[i];
		army[i] = army[temp];
		i = army[i];
		printf("\t\t no. %d kills\t",(army[i]-1));
	}
	printf("\nsurviver is number %d\n",(i-1));
	
	free(army);

	
}







