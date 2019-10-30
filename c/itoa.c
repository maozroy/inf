#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void reverse (char* str, size_t len);

char LUTable[36] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


char *itoa(int num, char* str, int base)
{
	char *temp_str = malloc(sizeof(char)*100);
	char *temp_base = temp_str;

	temp_str += 99;
	if(num < 0)
	{
		num *= -1;
		*temp_str = '-';
		--temp_str;
	}
	
	
	*temp_str = '\0';
	--temp_str;
	while (num>0)
	{
		*temp_str = LUTable[num % base];
		--temp_str;
		num /= base;
	}
	++temp_str;
	strcpy(str, temp_str);

	free(temp_base);
return str;
}


int main()
{
	int num = -123456789;
	char string[20] = "AaaaAaAAA";
	char *ptr = "";
	int i = 0;
	ptr = itoa(num, string, 12);
	printf("%s",string);


return 0;
}

void swap(char* a, char* b)
{
	char temp = *a;
	
	*a = *b;
	*b = temp;
}

void reverse (char* str, size_t len)
{
	size_t i = 0;
	
	for (i = 0 ; i < len/2 ; i++)
	{
		swap((str+i), (str+len-i));
	}

}
