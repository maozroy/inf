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
	
	for (i = 0 ; i < n ; i++)
	{
		army[i] = i+1;
	}
	army[n] = 0;
	i = 0;
	
	while (i != army[i])
	{
		next_guy = army[i];
		army[i] = army[next_guy];
		i = army[i];
		
		printf("\t\t no. %d kills\t",(army[i]-1));
	}
	printf("\nsurviver is number %d\n",(i-1));
	
	free(army);
}







