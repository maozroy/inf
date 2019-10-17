/**************************/
/*          WS5           */
/* Author: Maoz Roytman   */
/* Reviwer: ????          */
/*        16/10/19        */
/**************************/

#include <stdio.h>  /*FILE, EOF, prints, getc*/
#include <string.h> /*strlen*/
#include <stdlib.h> /*exit*/
enum output {working = 0,
			file_not_found = 1,
			file_not_closed = 2,
			file_not_opened = 3,
			file_not_removed = 4
			}status;

struct func_t
{
	char *op;			
	enum output (*op_func)();
	int (*comparing)(char* str, char *function);
};

enum output op_remove(char *str ,char *file); /*removes the file*/
enum output op_count(char *str ,char *src_file);  /*counts amount of lines in file*/
enum output op_exit();					      /*exits the program*/
enum output op_begin(char *str ,char *file);  /*writes to beginning of file*/
enum output op_write(char *str ,char *file);  /*writes to file*/
int blank_comparing (char* str, char *function);
int comparing(); 					  /*compares input to string commands*/
int begin_comparing (char* str, char *function);



/*when running the a.out, a declaration for a .txt file to output is needed.
Maximum of 256 chars per line*/

int main (int argc, char *argv[])
{


	struct func_t box[5] = {{"-remove", op_remove, comparing}, 
							{"-count", op_count, comparing}, 
							{"-exit", op_exit, comparing},
							{"<", op_begin, begin_comparing},
							{"", op_write, blank_comparing}};
	char input[256];
	int i = 0;
	int result = 0;

	while(1)
	{
		fgets (input, 100, stdin);
		do
		{
			if ((box[i].comparing (input, box[i].op))==0)
			{
				result = box[i].op_func(input, *(argv+1));
				if(result)
				{
					printf("\nError Code is %d\n",result);
				}
				break;
			}

		++i;
		} while (i < 5 && ((box[i-1].comparing (input, box[i-1].op)) != 0));
		i = 0;
	}
	return 0;
}

int comparing (char* str, char *function)
{
	return strncmp(function, str, strlen(function)-1);
}

int begin_comparing (char* str, char *function)
{
	return strncmp(function, str, strlen(function));
}

int blank_comparing (char* str, char *function)
{
	return 0;
}

enum output op_write (char *str ,char *file)
{
	int status = 0;
	FILE *input;
	printf("input for write function is %s\n", str);
	input = fopen(file, "a+");
	fputs(str, input);
	status = (fclose(input) == 0) ? working : file_not_closed ;
	return (status);
}

enum output op_remove (char *str ,char *file)
{
	int status = 0;
	printf("this is remove function");
	status = (remove(file)==0) ? working : file_not_removed ;
	return (status);
}

enum output op_exit ()
{
	exit(0);
	return 0;
}

enum output op_count (char *str ,char *file)
{
	char c = 0;
	char counter = 0;
	int status = 0;
	FILE *input = fopen(file, "r");;

	*str = 0;
	if (input == NULL)
		{
			return file_not_opened;
		}
	for (c = getc(input) ; c!= EOF ; c = getc(input))
	{
		if (c == '\n')
		{
			++counter;
		}
	}
	printf("file has %d lines\n", counter);
	status = (fclose(input) == 0) ? working : file_not_closed ;
	return (status);
}

enum output op_begin (char *str ,char *src_file)
{

	FILE *input;
	FILE *temp_f;
	char ch;
	
	printf("input for begin function is %s\n", str);
	temp_f = fopen("dummy.txt", "w+");
	input = fopen(src_file, "r");
	
	if (input == NULL)
	{
		printf("CHECK");
		return file_not_opened;
	}
	fputs((str+1), temp_f);
	
	while((ch = (fgetc(input))) != EOF)
	{
		printf("looping\n");
		fputc(ch, temp_f);
	}
	rename("dummy.txt", src_file);
	fclose(input);
	fclose(temp_f);
	return 0;
}
