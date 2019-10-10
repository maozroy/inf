<<<<<<< HEAD
/************************/
/*          WS3         */
/* Author: Maoz Roytman */
/* Reviwer: Gal Salemon */
/*        9/10/19       */
/************************/

#include <stdio.h>  /*for printf*/
#include <string.h> /*for strcpy*/
#include <stdlib.h> /*for calloc, free*/
#include <ctype.h>  /*for tolower*/

void FreeBack(char **ptr);             /*memory freeing of ** */
void PrintArray(char **arr);           /*printing strings within ** */
void CopyArray(char **arr);            /*copis a ** pointer to a new temp ** pointer  */
void Joseph(int n);                    /*resolves Josephus problem*/
char *MyCalloc(int size, int blocker); /*allocates memory and return null on certain number*/
void PrintDataTypes();

int main (int argc, char* argv[], char *env[]) 
{
	int suiciders = 0;

	argc += 1;
	**argv = 1;

	printf("Please enter number of suiciders:");
	scanf("%d", &suiciders);
	Joseph(suiciders);
	
	PrintDataTypes();
	CopyArray(env);	

	return 0;
}

void PrintDataTypes()
{

	puts("\t\tCHARS");
	printf("size of (unsigned) char    = %lu bytes\n", sizeof(char));
	printf("size of signed char        = %lu bytes\n", sizeof(signed char));

	puts("\n\t\tINTS");
	printf("size of (signed) short int = %lu bytes\n", sizeof(signed short int));
	printf("size of unsigned short int = %lu bytes\n", sizeof(unsigned short int));
	printf("          -----                    \n");
	printf("size of (signed) int       = %lu bytes\n", sizeof(int));
	printf("size of unsigned int       = %lu bytes\n", sizeof(unsigned int));
	printf("          -----                    \n");
	printf("size of (signed) long int  = %lu bytes\n", sizeof(signed long int));
	printf("size of unsigned long int  = %lu bytes\n", sizeof(unsigned long int));

	puts("\n\t\tFLOATS");
	printf("size of float              = %lu bytes\n", sizeof(float));
	printf("size of long float         = %lu bytes\n", sizeof(long));
	printf("size of double float       = %lu bytes\n", sizeof(double));
	printf("size of long double float  = %lu bytes\n", sizeof(long double));
	puts("\n\t\tPOINTER");
	printf("size of pointer            = %lu bytes\n", sizeof(int*));
}


void PrintArray(char **arr)
{
	char **arr_start = arr;

	while(*arr_start)
	{
		printf("%s \n", *arr_start);
		arr_start++;
	}
	arr = arr_start;
}

void CopyArray(char **arr)
{
	int string_len = 0;
	int arr_index = 0;
	int i = 0;
	int Nuller = 0;
	
	char **arr_start = arr;
	char **allocated_arr = NULL;
	char **allocated_arr_start;
	
	while(*arr)
	{
		arr_index++;	
		arr++;
	}
	arr = arr_start;
	
	allocated_arr = calloc(arr_index+1,(sizeof(char*)));
	allocated_arr_start = allocated_arr;

		while (*arr)
		{
			string_len = strlen(*arr);
			*allocated_arr = MyCalloc(string_len, Nuller);
			++Nuller;
			
			/*Nuller limit will Nullify pointer, thus causing memory allocation to "fail" */
			if(*allocated_arr == NULL) 
			{	
				FreeBack(allocated_arr_start); 
				return;
			}
			strcpy(*allocated_arr, *arr); 
			
			for (i = 0 ; i < (string_len+1) ; i++)
			{
				*(*allocated_arr+i) = tolower(*(*allocated_arr+i));
			}
			
			allocated_arr++;
			arr++;
		}
	arr = arr_start;
	
	PrintArray(allocated_arr_start);
	FreeBack(allocated_arr_start);
}

void FreeBack(char **ptr)
{
	char **ptr_start;
	
	ptr_start = ptr;
	
	while(*ptr)
	{
		free(*ptr);
		ptr++;
	}

	ptr = ptr_start;
	free(ptr);
		
	return;
}

char *MyCalloc(int size, int nuller)
{
	if (nuller < 100)
	{
		return calloc(size+1,(sizeof(char)));
	}
	else
	{
		return NULL;
	}
}

void Joseph(int n) /* n is number of soldiers */
{
	int *army;     /* entire array */
	int i = 0;     /* current soldier */
	int next_guy = 0; 
	
	army = malloc(sizeof(int) * (n + 1));
=======
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
>>>>>>> aa82784baaedbdd30448241c3985bb7ae685ca6b
	
	for (i = 0 ; i < n ; i++)
	{
		army[i] = i+1;
	}
	army[n] = 0;
	i = 0;
	
	while (i != army[i])
	{
<<<<<<< HEAD
		next_guy = army[i];
		army[i] = army[next_guy];
		i = army[i];
		
=======
		temp = army[i];
		army[i] = army[temp];
		i = army[i];
>>>>>>> aa82784baaedbdd30448241c3985bb7ae685ca6b
		printf("\t\t no. %d kills\t",(army[i]-1));
	}
	printf("\nsurviver is number %d\n",(i-1));
	
	free(army);
<<<<<<< HEAD
=======

	
>>>>>>> aa82784baaedbdd30448241c3985bb7ae685ca6b
}







