#include <stdio.h> /*for printf */
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX 4096


size_t mystrlen(char *src) /*defining func that recieved ptr to char array and returnes size_t*/
{
	int len = 0; /* i counts array cells */
	
	while ('\0' != *src)
	{
		++len;
		++src;
	}

	return  len;
}

int mystrcmp(const char *str1, const char *str2)
{
	while (( '\0' != *str1) || ('\0' != *str2 )) /*check if array is over*/
	{
		if ((int) *str1 == (int) *str2) /*check if ascii is value equal and move to next element*/
		{
			++str1; 
			++str2;			
		}
		else
		{
			return ( (int) *str1 - (int) *str2); /*return comparison*/
		}
	}

	return 0; 
}

int mystrcasecmp(const char *str1, const char *str2)
{
	while (( '\0' != *str1) || ('\0' != *str2 )) /*check if array is over*/
	{
		if ((tolower(*str1)) == (tolower(*str2))) /*check if ascii is value equal and move to next element*/
		{
			++str1; 
			++str2;			
		}
		else
		{
			return (tolower(*str1) - tolower(*str2)); /*return comparison*/
		}
	}

	return 0; 
}



char *mystrcpy(char *dest,const char *src) /*crop entire string to another string*/
{
	char *temp = dest; /*saves original pointer to destination array*/
	assert(NULL != src);
	while ('\0' != *src) /*checks if source is over or nah*/
	{
		*dest = *src; /*copy values */
		++dest;
		++src;
	}
	*dest='\0';

	return temp;
}


char *mystrncpy(char *dest, char *src, size_t n)
{
	char *temp = dest;
	size_t i = 0;
	assert(*src);
	while ((i < n) && ('\0' != *src))
	{
		*dest = *src;
		++dest;
		++src;
		++i;
	}
	*dest='\0';

	return temp;
}

char* mystrchr( char *src, char c)
{
	while (*src != '\0')
	{
		if ( *src != c ) 
		{
			src++;
		}
		else
		{
			if (*src == c)
			{
				return src; 
			}
		}
	}
	
	return NULL; 
}

char* mystrdup(const char *src) /* duplicates existing string to new one */
{
	char *str; /* ptr to hold beginning of new string */
	char *p; /* ptr same as str but is incremanted every round */
	int len = 0; /* int to count length of source string */

	while (src[len])
	{
		len++;
	}
	str = malloc(len+1);
	p = str;
	while (*src)
	{
		*p = *src;
		p++;
		src++;
	}
	*p = '\0'; /* declare end of array */
	return str;
	
}

void testmystrdup()
{
	char testarray1[MAX] = "ABC";
	char testarray3[MAX] ="";
	char testarray5[MAX] ="k0s0m0";
	char *ptr =mystrdup(testarray1);
	printf("\n\t\tmystrdup testing\n");

	if ('A' == *(ptr))
	{
		printf("ABC SUCSEED\n");
	}
	else 
	{
		printf("ABC FAILED\n");
	}
	free(ptr);
	ptr = mystrdup(testarray3);
	if (' ' == *(ptr))
	{
		printf("space SUCSEED\n");
	}
	else 
	{
		printf("space FAILED\n");
	}
	free(ptr);
	ptr = mystrdup(testarray5);
	if ('k' == *(ptr))
	{
		printf("kos SUCSEED\n");
	}
	else 
	{
		printf("kos FAILED\n");
	}
	free(ptr);
}


void testmystrlen()
{
	char testarray[MAX] = "ABC";
	char testarray2[MAX] = "\0";
	char testarray3[MAX] ="012345678";
	printf("\n\t\tstrlen testing\n");

	if (strlen(testarray) == mystrlen(testarray))
	{
		printf("ABC SUCSEED\n");
	}
	else 
		printf("ABC FAILED\n");


	if (strlen(testarray2) == mystrlen(testarray2))
	{
		printf("0 SUCSEED\n");
	}
	else 
		printf("0 FAILED\n");


	if (strlen(testarray3) == mystrlen(testarray3))
	{
		printf("Numbers check SUCSEED\n");
	}
	else 
		printf("Numbers check FAILED\n");
}


void testmystrchr()
{
	char testarray1[MAX] = "AB M";
	char testarray2[MAX] = "maMa";
	char testarray3[MAX] ="012345678";
	char test1 = ' ';
	char test2 = 'M';
	char test3 = '0';
	printf("\n\t\ttest mysrtchr testing\n");

	if (strchr(testarray1, test1) == mystrchr(testarray1, test1))
	{
		printf("space test SUCSEED\n");
	}
	else 
		printf("space test  FAILED\n");


	if (strchr(testarray2, test2) == mystrchr(testarray2, test2))
	{
		printf("0 SUCSEED\n");
	}
	else 
		printf("0 FAILED\n");


	if (strchr(testarray3, test3) == mystrchr(testarray3, test3))
	{
		printf("Numbers check SUCSEED\n");
	}
	else 
		printf("Numbers check FAILED\n");
}


void testmystrcmp()
{
	char testarray1[MAX] = " ";
	char testarray2[MAX] = "M";
	char testarray3[MAX] = "\0";
	char testarray4[MAX] = " ";
	char testarray5[MAX] = "maoz";
	char testarray6[MAX] = "fork";
	printf("\n\t\tstrcmp testing\n");
	if (strcmp(testarray1, testarray2) == mystrcmp(testarray1, testarray2))
	{
		printf("empty array SUCSEED\n");
	}
	else 
		printf("%d\n",mystrcmp(testarray1, testarray2));



	if (strcmp(testarray3, testarray4) == mystrcmp(testarray3, testarray4))
	{
		printf("NULL SUCSEED\n");
	}
	else 
		printf("%d\n",strcmp(testarray3, testarray4));



	if (strcmp(testarray5, testarray6) == mystrcmp(testarray5, testarray6))
	{
		printf("maoz fork SUCSEED\n");
	}
	else 
		printf("%d\n",mystrcmp(testarray5, testarray6));
}

void testmystrcasecmp()
{
	char testarray1[MAX] = "1";
	char testarray2[MAX] = "M";
	char testarray3[MAX] = "\0";
	char testarray4[MAX] = " ";
	char testarray5[MAX] = "MAOZ";
	char testarray6[MAX] = "maoz";
	printf("\n\t\tstrcasecmp testing\n");
	if (strcasecmp(testarray1, testarray2) == mystrcasecmp(testarray1, testarray2))
	{
		printf("empty array SUCSEED\n");
	}
	else 
{
		printf("%d\n",strcasecmp(testarray1, testarray2));
		printf("%d\n",mystrcasecmp(testarray1, testarray2));

}

	if (strcasecmp(testarray3, testarray4) == mystrcasecmp(testarray3, testarray4))
	{
		printf("NULL SUCSEED\n");
	}
	else 
		printf("%d\n",strcmp(testarray3, testarray4));



	if (strcasecmp(testarray5, testarray6) == mystrcasecmp(testarray5, testarray6))
	{
		printf("MAOZ maoz SUCSEED\n");
	}
	else 
		printf("%d\n",mystrcmp(testarray5, testarray6));
}


void testmystrcpy()
{
/*1st string is destination, 2ns string is src */


	char testarray1[MAX] = "roytman";
	char testarray2[MAX] = "maoz";

	char testarray3[MAX] = "\0";
	char testarray4[MAX] = "man";

	char testarray5[MAX] = "maoz";
	char testarray6[MAX] = "forking";
	printf("\n\t\tstrcpy testing\n");
	if (testarray1 == mystrcpy(testarray1, testarray2))
	{
		printf("longer dest string SUCSEED\n");
	}
	else 
		printf("%s\n",testarray1);



	if (testarray3 == mystrcpy(testarray3, testarray4))
	{
		printf("NULL SUCSEED\n");
	}
	else 
		printf("%s\n",mystrcpy(testarray3, testarray4));



	if (testarray5 == mystrcpy(testarray5, testarray6))
	{
		printf("Longer Src SUCSEED\n");
	}
	else 
		printf("%s\n",mystrcpy(testarray5, testarray6));
}

void testmystrncpy()
{
/*1st string is destination, 2ns string is src */

	int limit = 4;

	char testarray1[MAX] = "roytman";
	char testarray2[MAX] = "maoz";

	char testarray3[MAX] = "\0";
	char testarray4[MAX] = "man";

	char testarray5[MAX] = "maoz";
	char testarray6[MAX] = "forking";

	printf("\n\t\tmystrncpy testing\n");
	if (testarray1 == mystrncpy(testarray1, testarray2, limit))
	{
		printf("longer dest string SUCSEED\n");
	}
	else 
		printf("%s\n",testarray1);



	if (testarray3 == mystrncpy(testarray3, testarray4, limit))
	{
		printf("NULL SUCSEED\n");
	}
	else 
		printf("%s\n",mystrncpy(testarray3, testarray4, limit));



	if (testarray5 == mystrncpy(testarray5, testarray6, limit))
	{
		printf("Longer Src SUCSEED\n");
	}
	else 
		printf("%s\n",mystrncpy(testarray5, testarray6, limit));
}


int main()
{



	testmystrlen();
	testmystrcmp();
	testmystrcpy();
	testmystrcasecmp();
	testmystrchr();
	testmystrdup();
	


	return 0;
}
