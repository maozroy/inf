/**************************
*       Maoz Roytman
*      Tamir Dayan
*        24/10/19
**************************/


#include <stdio.h> /*for printf*/
#include <stdlib.h> /*for malloc/free*/
#include <string.h> /*for strcpy*/
#include <assert.h> /*for assert*/

#define MAX2(a, b) ((a > b) ? a : b) /*compares 2 nums*/
#define MAX3(a, b, c) (MAX2(a, b) < c) ? c : MAX2(a, b) /*compares 3 numbers*/
#define mysize(a) ((char*)(&a+1) - (char*)&a) /*check sizeof*/
#define mysize_type(type)  (long) (( type*) 0 + 1) /*check sizeof var*/
#define NUM_OF_ELEMENTS 3 /*in array*/
#define MAX_STRING 10 /*max len of string*/
#define TEST(no, result) (printf("\nTest %d %s\n", no, 1 == result ? "PASS" : "FAILED"))

struct type
{
	void *data; /*main array*/
	void (*print_func) (void* data);
	void (*addition) (void** data, int num_to_add);
	unsigned int heap_flag : 1; /*checks if malloc used*/
};
/*print funcs*/
void print_int (void* data);
void print_float (void* data);
void print_string (void* data);
/*addition funcs*/
void add_int (void** data, int num_to_add);
void add_float (void** data, int num_to_add);
void add_string (void** data, int num_to_add);

void LoopToFree (struct type st_ptr[]); /*memory relese func*/
void macro_test_unit();
void sizeof_test_unit();

void macro_test_unit()
{
	int result = 0;
	
	printf("\t\t\tMAX3 Test\n");
	(MAX3(1, 20, 0) == 20) ? result = 1 : (result = 0);
	TEST(1, result);
	(MAX3(100000, -2 , 0) == 100000) ? result = 1 : (result = 0);
	TEST(2, result);
	(MAX3(0, 1000 , 1000) == 1000) ? result = 1 : (result = 0);
	TEST(3, result);
}

void sizeof_test_unit()
{
	int result = 0;
	
	printf("\t\t\tsizeof Test\n");
	((mysize(result) == sizeof(result)) ? result = 1 : (result = 0));
	TEST(1, result);
	((mysize_type(long) == sizeof(long)) ? result = 1 : (result = 0));
	TEST(2, result);
	(((mysize_type(char*)) == sizeof(int*)) ? result = 1 : (result = 0));
	TEST(3, result);
}

void print_int (void* data)
{
	assert(data);
	
	printf("\nint is: %d\n", *( (int*) & data) );
}

void print_float (void* data)
{
	assert(data);
	
	printf("\nfloat is: %f\n", *( (float*) & data) );
}

void print_string (void* data)
{
	assert(data);
	
	printf("\nstring is: %s\n", (char*)data );
}

void add_int (void** data, int num_to_add)
{
	assert(data);
	
	*((int*) (data) ) += num_to_add;
}

void add_float (void** data, int num_to_add)
{
	assert(data);
	
	*((float*) (data) ) += num_to_add;
}

void add_string (void** data, int num_to_add)
{
	assert(data);
	*data = realloc(*data, MAX_STRING+2);
	
	sprintf(*data, "%s%d", (char*)*data , num_to_add);
}

void LoopToFree (struct type st_ptr[])
{
	int i = 0;
	
	while (i < NUM_OF_ELEMENTS)
	{
		if (1 == st_ptr[i].heap_flag)
		{
			free(st_ptr[i].data);
		}
		++i;
	}
}

int main()
{
	int num_to_add = 10;
	char *string = 0;
	int i = 0;
	
	struct type array[NUM_OF_ELEMENTS];
	/*initalizing flag*/
	array[0].heap_flag = 0;
	array[1].heap_flag = 0;
	array[2].heap_flag = 1;
	
	array[0].print_func = print_int;
	array[1].print_func = print_float;
	array[2].print_func = print_string;
	
	array[0].addition = add_int;
	array[1].addition = add_float;
	array[2].addition = add_string;

	string = malloc(sizeof(char)*MAX_STRING);
	if (NULL == string)
	{
		printf("\nMemory Allocation Failed!!!\n");
		exit(0);
	}
	strcpy(string,"123456789");
	
	/*definition of values*/
	array[0].data = (int*)4;
	*( (float*) & (array[1].data) ) = 4.2324896;
	array[2].data = string;
	/*loop for print*/
	for (i = 0; i < NUM_OF_ELEMENTS ; i++)
	{
		array[i].print_func(array[i].data);
	}

	/*loop for addition*/
	for (i = 0 ; i < NUM_OF_ELEMENTS ; i++)
	{
		array[i].addition(&array[i].data, num_to_add);
	}
	/*loop for print*/
	for (i = 0 ; i < NUM_OF_ELEMENTS ; i++)
	{
		array[i].print_func(array[i].data);
	}
	LoopToFree(array);
	macro_test_unit();
	sizeof_test_unit();

	return (0);
}
