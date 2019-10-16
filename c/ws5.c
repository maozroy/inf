#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int op_remove(char *str ,char *file);
int op_count(char *str ,char *file);
int op_exit();
int op_begin(char *str ,char *file);
int op_write(char *str ,char *file);
int comparing();
struct func_t
{
	char *op;
	int (*op_func)();
	int (*comparing)(char* str, char *function);
};

int main(int argc, char *argv[])
{

	struct func_t box[5] = {{"-remove", op_remove, comparing}, 
							{"-count", op_count, comparing}, 
							{"-exit", op_exit, comparing},
							{"<", op_begin, comparing},
							{"", op_write, comparing}};
	char input[256];
	int i = 0;

	
	while(1)
	{
		fgets(input, 100, stdin);
		do
		{
			if((box[i].comparing (input, box[i].op))==0)
			{
				box[i].op_func(input, *(argv+1));
			}

		++i;
		}while (i < 5 && ((box[i-1].comparing (input, box[i-1].op)) != 0));
		i = 0;
	}
	return 0;
}




int comparing(char* str, char *function)
{
	return strncmp(function, str, strlen(function));
}

int op_write(char *str ,char *file)
{

	FILE *input;
	input = fopen(file, "a+");
	fputs(str, input);
	fclose(input);
	return 0;
}

int op_remove(char *str ,char *file)
{
	remove(file);
	return 0;
}

int op_exit()
{
	exit(0);
	return 0;
}

int op_count(char *str ,char *file)
{
	char c = 0;
	char counter = 0;
	FILE *input;
	*str = 0;
	input = fopen(file, "r");
	for(c = getc(input) ; c!= EOF ; c = getc(input))
	{
		if (c == '\n')
		{
			++counter;
		}
	}
	printf("file has %d lines\n", counter);
	fclose(input);
	return 0;
}

int op_begin(char *str ,char *file)
{
	FILE *input;
	str++;
	printf("this is op op_begin\n");
	input = fopen(file, "r+");
	fseek(input, 0L, SEEK_SET);
	fputs(str, input);
	fclose(input);
	return 0;
}
