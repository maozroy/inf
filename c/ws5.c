/*****************************/
/*          WS5              */
/* Author: Maoz Roytman      */
/* Reviwer: Yonatan Vologdin */
/*        19/10/19           */
/*****************************/

#include <stdio.h>  /*FILE, EOF, prints, getc*/
#include <string.h> /*strlen*/
#include <stdlib.h> /*exit*/

#define MAX_INPUT 256 
#define NUM_OF_OPS 5 /*num of different operations*/

enum output {working = 0,
			file_not_found = 1,
			file_not_closed = 2,
			file_not_opened = 3,
			file_not_removed = 4
			}status;
	
struct func_t
{
	char *op; /*string to activate operation*/			
	enum output (*op_func)(); /* operation itself */
	int (*comparing)(char* str, char *function); /*cmp op and usr input*/
};

enum output op_remove(char *str ,char *file); /*removes the file*/
enum output op_count(char *str ,char *src_file);  /*counts lines in file*/
enum output op_exit();					      /*exits the program*/
enum output op_begin(char *str ,char *file);  /*writes to beginning of file*/
enum output op_write(char *str ,char *file);  /*writes to file*/
int blank_comparing (char* str, char *function); /*always 0 */
int comparing(); 					  /*cmp input to string commands*/
int begin_comparing (char* str, char *function); /*cmp first char only*/

/*when running the a.out, a declaration for a .txt file to output is needed.
Maximum of 256 chars per line*/

int main (int argc, char *argv[])
{
	struct func_t box[NUM_OF_OPS] = {{"-remove", op_remove, comparing}, 
							{"-count", op_count, comparing}, 
							{"-exit", op_exit, comparing},
							{"<", op_begin, begin_comparing},
							{"", op_write, blank_comparing}};
	char input[MAX_INPUT] = "";
	int i = 0;
	int result = 0;
	
	(void)argc;
	printf("\t\t\twelcome to the write program\n");
	printf("-remove will delete your file\n");
	printf("-count will count lines in file\n");
	printf("-exit will exit the program\n");
	printf("< in beginning of string will write to beginning of file\n");
	printf("all other strings will write to end of file\n");
	
	while(1)
	{
		fgets (input, MAX_INPUT, stdin);
		input[strlen(input)-1]='\0';
		do
		{
			if (0 == (box[i].comparing (input, box[i].op)))
			{
				result = box[i].op_func(input, *(argv+1));
				if (0 != result)
				{
					printf("\nError Code is %d\n",result);
				}
				break;
			}
		++i;
		} while (i < NUM_OF_OPS && ((box[i-1].comparing (input, box[i-1].op)) != 0));
		i = 0;
	}
	
	return working;
}

int comparing (char* str, char *function)
{
	return strcmp(function, str);
}

int begin_comparing (char* str, char *function)
{

	return strncmp(function, str, strlen(function));
}

int blank_comparing (char* str, char *function)
{
	(void)str;
	(void)function;
	
	return 0;
}

enum output op_write (char *str ,char *file)
{
	int status = 0;
	FILE *input;
	
	if (!(file))
	{
		status = file_not_found;
		
		return (status);
	}
	input = fopen(file, "a+");
	if(fgetc(input) != '\n' && (fgetc(input) != EOF) )
	{
		fputs("\n", input);
	}
	fputs(str, input);
	status = (0 == fclose(input)) ? working : file_not_closed;
	
	return (status);
}

enum output op_remove (char *str ,char *file)
{
	int status = 0;
	
	(void)str;
	printf("Remove File\n");
	status = (remove(file)==0) ? working : file_not_removed;
	return (status);
}

enum output op_exit ()
{
	exit(0);
}

enum output op_count (char *str ,char *file)
{
	char ch_check = 0;
	char num_of_lines = 0;
	int status = 0;
	FILE *input = fopen(file, "r");

	(void)str;
	if (NULL == input)
		{
		
			return file_not_opened;
		}
	for (ch_check = getc(input) ; ch_check!= EOF ; ch_check = getc(input))
	{
		if (ch_check == '\n' && )
		{
			++num_of_lines;
		}
	}
	printf("file has %d lines\n", num_of_lines);
	status = (fclose(input) == 0) ? working : file_not_closed ;
	return (status);
}

enum output op_begin (char *str ,char *src_file)
{

	FILE *input;
	FILE *temp_f;
	char ch;
	status = 0;
	
	printf("Write to Beginning of file\n");
	temp_f = fopen("dummy.txt", "a");
	input = fopen(src_file, "r+");
	
	if (NULL == input)
	{
	
		return file_not_opened;
	}
	
	fputs((str+1), temp_f);
	fputs ("\n",temp_f);
	while((ch = (fgetc(input))) != EOF)
	{
		fputc(ch, temp_f);
	}
	rename("dummy.txt", src_file);
	status = (fclose(input) == 0) ? working : file_not_closed;
	status = (fclose(temp_f) == 0) ? working : file_not_closed;
	
	return (status);
}
