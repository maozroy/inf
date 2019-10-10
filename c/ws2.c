/*********************************/
/*           worksheet2          */
/* Author: Maoz Roytman          */
/* Reviwer: Yonatan Vologdin     */
/* Date 06/10/19                 */
/*********************************/

#include <stdlib.h> /*for mallocs */
#include <stdio.h> /*for prints */
#include <string.h> /*for strlen */

int is_palindrome (char *str);
int containdigit(int num);
void boom(int first, int last);
void adding(char* num1, char* num2);
void reverse(char *str);
void test_is_palindrome();
void swap(int *a, int *b);

int main()
{
	char str1[] = "199";
	char str2[] = "199";
	int a = 10;
	int b = 100;

	test_is_palindrome();
	boom(a, b); 
	adding(str1, str2);
	swap(&a, &b);



	/*RmSpaces(string); */

	return 0;
}

void adding(char* num1, char* num2)
{
	char* result;
	char* free_the_pointer = NULL	;
	int len1 = strlen(num1);
	int len2 = strlen(num2);
	int i = 0;
	int carry = 0;
	char tempresult = 0;
	int max = 0;
	
	result = malloc( sizeof(char*) * ((len1 > len2) ? len1 : len2) + 2);
	if(NULL == result)
	{
		return;
	}
	free_the_pointer = result;
	max = (len1 > len2) ? len1 : len2; 
	reverse(num1);
	reverse(num2);
	
	for(i = 0 ; i < max ; i++)
	{
		if(i < len1) 
		{
			tempresult += (*(num1+i)-48); 
		}
		
		if(i < len2)
		{
			tempresult += (*(num2+i)-48);
		}
		
		tempresult += carry;
		*result = tempresult % 10;
		carry =  tempresult / 10;
		tempresult = 0;
		result++;
	}
	
	if(carry)  /*add the carry if exists*/
	{
		*result = carry;
	}
	
	i = 0;
	result--;
	printf("\n\nresult of addition is: ");
	while(i < (max+carry)) /*print the array from end to beginning*/
	{
		printf("%d",*result);
		result--;
		i++;
	}
	free(free_the_pointer);
	
}

void reverse(char *str)
{
	char *begin = 0;
	char *end = 0;
	char temp;
	int len = strlen(str);
	int i = 0;
	begin = str;
	end = str;

	for( ; i < (len-1) ; i++)
	{
		end++;
	}

	for(i = 0 ; i < (len/2) ; i++)
	{
		temp = *begin;
		*begin = *end;
		*end = temp;
		--end;
		++begin;
	}
}

int is_palindrome (char *str)
{
	int i = 0;
	char *str_start = str;
	while (*str)
	{
		++str;
		++i;
	}
	--str;
	while ((i/2) > 0)
	{
		if(*str_start != *str)
			{
				return 0;
			}
		--i;
		++str_start;
		--str;	
	}
	return 1;
}

int containdigit(int num)
{	
	while(num != 0)
	{
		if(7 == num%10)
		{
			return 1;
		}else
		num = num/10;
	}
	return 0;
	
}

void boom(int first, int last)
	{
	while (first < last)
	{
		if (((first %7) != 0) || (first == 0))
			{
			if(containdigit(first)==0)
				{
					printf("%d, ",first);
				}
			}
		else
		{
			printf("BOOM ");
		}
		++first;
	}
		printf("\n\n\t\tthe end of BOOM ");
}

void test_is_palindrome()
	{
		char testarray[40]="maozoam";
		printf("\n\nfunction is:%d\n\n",is_palindrome(testarray));
	}
	
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
	
/*void RmSpaces(char *str)
{
	char *str_w = str;
	while( isspace(*str) )
	{
		++str;
		
	}
	while(*str != '\0')
	{
		if ((*str != ' ') || 
			(	(*str == ' ') && (*(str_w-1) != ' ')	) )
			{

				*str_w=*str;
				printf("str is %c\t str_w is %c\n", *str, *str_w);
				++str;
				printf("str moved\n");
				++str_w;
				printf("str_w moved\n");

			}
		else
		{
			++str;
				printf("str (else) moved\n");
		}
	}

	while( isspace(*str) )
	{
		++str;
						printf("str (last while) moved\n");

	}
	*(str_w+1) = '\0';
}*/






