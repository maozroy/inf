
	#include <stdio.h>	
	#include <stdlib.h>	
	#include <string.h>
	#include <assert.h> 
	#include <ctype.h>
	#include <strings.h>

	/* *** Functions Declaration *** */

	/* calculates the length of the string pointed to by s, 
	excluding the terminating null byte ('\0'). */
	
	size_t MyStrlen(const char *s); 

	/* compares the two strings s1 and s2.  It returns
       an integer less than, equal to, or greater than zero if  s1  is  found,
       respectively, to be less than, to match, or be greater than s2. */
	
	int MyStrcmp(const char *str1, const char *str2);

	/* copies the string pointed to by src, including
       	the terminating null byte ('\0'), to the buffer  pointed  to  by  dest.
      	The  strings  may  not overlap, and the destination string dest must be
      	large enough to receive the copy.  Beware of buffer overruns! */
	
	char *MyStrcpy(char *dest, const char *source);

	/* similar to Mystrcpy(), except that at most n bytes of src
     	are copied.  Warning: If there is no null byte among the first n bytes
       	of src, the string placed in dest will not be null-terminated.

       	* If  the  length of src is less than n, strncpy() writes additional null
       	bytes to dest to ensure that a total of n bytes are written. */
	
	char *MyStrncpy(char *dest, const char *source, size_t n);

	/* compare two strings ignoring case. performs a byte-by-byte comparison of the
      	 strings s1 and s2, ignoring the case of the characters.  It returns  an
      	 integer  less  than,  equal  to,  or  greater than zero if s1 is found,
   	 respectively, to be less than, to match, or be greater than s2. */
	
	int MyStrcasecmp(const char *str1, const char *str2);

	/*  locate character in string - returns a pointer to the first
	occurrence of the character c in the string s. 
	* returns NULL if the character is not found.  
	* The terminating null byte is considered part of the string, so that
	if locate_char  is  specified  as '\0', these functions return a
	pointer to the terminator. */
	
	char *MyStrchr(const char *str, int locate_char);

	/*
	function  returns  a  pointer to a new string which is a
      	duplicate of the string s.  Memory for the new string is obtained  with
      	malloc(), and can be freed with free().
	It returns NULL if insufficient  memory  was  available. */
	
	char *MyStrdup(const char *str);

	/* appends the src string to the dest string, over‐writing the 
	terminating null byte ('\0') at the end of dest,  and  then
       	adds  a  terminating  null  byte.  
	* The strings may not overlap, and the dest string must have enough
	 space for the  result. If dest is not large enough, program behavior
	 is unpredictable. */
	
	char *MyStrcat(char *dest, const char *src);

	/* similar to strcat(), except that:
       	*  it will use at most n bytes from src; and
       	*  src does not need to be null-terminated if it  contains  n  or  more
          bytes.
       	As  with  strcat(), the resulting string in dest is always null-termi‐
       nated.

       If src contains n or more bytes, strncat() writes n+1 bytes to dest (n
       from  src plus the terminating null byte).  Therefore, the size of dest
       must be at least strlen(dest)+n+1. */

	char *MyStrncat(char *dest, const char *src, size_t n);

	void TestMyStrlen(); /* Testing the function Mystrlen() */
	void TestMyStrcmp(); /* Testing the function MyStrcmp() */
	void TestMyStrcpy(); /* Testing the function MyStrcpy() */
	void TestMyStrncpy(); /* Testing the function MyStrncpy() */
	void TestMyStrcasecmp(); /* Testing the function MyStrcasecmp() */
	void TestMyStrchr(); /* Testing the function MyStrchr() */
	void TestMyStrdup(); /* Testing the function MyStrdup() */
	void TestMyStrcat(); /* Testing the function MyStrcat() */
	void TestMyStrncat(); /* Testing the function MyStrncat() */

	/* *** End of Functions Declaration *** */
		

	int main ()
	{
		TestMyStrlen(); 
		TestMyStrcmp(); 
		TestMyStrcpy(); 
		TestMyStrncpy(); 
		TestMyStrcasecmp(); 
		TestMyStrchr(); 
		/*TestMyStrdup(); */
		TestMyStrcat();
		TestMyStrncat();

		return 0;
	}


	void TestMyStrlen()
	{
		char str1[4] = "abc";
		
		if (strlen(str1) == MyStrlen(str1))
		{
			printf("Mystrlen: str1: PASS\n");
		}

		else
		{
			printf("Mystrlen: str1: FAIL\n");
		}
		
	}
	
	void TestMyStrcmp()
	{

		char str1[4] = "abc";
		char str2[4] = "aBc";
		char str3[4] = "abc";
		
		if (strcmp(str1, str2) == MyStrcmp(str1, str2))
		{
			printf("MyStrcmp: str1 + str2: PASS\n");
		}

		else
		{
			printf("MyStrcmp: str1 + str2: FAIL\n");
		}

		if (strcmp(str1, str3) == MyStrcmp(str1, str3))
		{
			printf("MyStrcmp: str1 + str3: PASS\n");
		}

		else
		{
			printf("MyStrcmp: str1 + str3: FAIL\n");
		}
		
	}
		
	void TestMyStrcpy()
	{
		char str1[4] = "abc";
		char str2[4] = "";

		if (strcpy(str1, str2) == MyStrcpy(str1, str2))
		{
			printf("MyStrcpy: PASS\n");
		}

		else
		{
			printf("MyStrcpy: FAIL\n");
		}
		
	}

	void TestMyStrncpy() /* Testing the function MyStrncpy() */
	{

		char str1[4] = "abc";
		char str2[4] = "";
		size_t n = 2;


		if (strncpy(str1, str2, n) == MyStrncpy(str1, str2, n))
		{
			printf("MyStrncpy: str1: PASS\n");
		}

		else
		{
			printf("MyStrncpy: str1: FAIL\n");
		}
		
	}


	void TestMyStrcasecmp()
	{
		char string1[4] = "abc";
		char string2[4] = "aBc";
		char string3[4] = "abd";
		
		if (strcasecmp(string1, string2) == MyStrcasecmp(string1, string2))
		{
			printf("MyStrcasecmp: str1 + str2: PASS\n");
		}

		else
		{
			printf("MyStrcasecmp: str1 + str2: FAIL\n");
		}

		if (strcasecmp(string1, string3) == MyStrcasecmp(string1, string3))
		{
			printf("MyStrcasecmp: str1 + str3: PASS\n");
		}

		else
		{
			printf("MyStrcasecmp: str1 + str3: FAIL\n");
		}

		
	}


	void TestMyStrchr()
	{
		const char str[] = "This is a String"; 
		const char ch = 'a'; 

		if (strchr(str, ch) == MyStrchr(str, ch))
		{
			printf("MyStrchr: PASS\n");
		}

		else
		{
			printf("MyStrchr: FAIL\n");
		}
	
	}

	/*void TestMyStrdup()
	{
		const char source[] = "This is a String";

		if (strdup(source) == MyStrdup(source))
		{
			printf("MyStrdup: PASS\n");
		}

		else
		{
			printf("MyStrdup: FAIL\n");
		}
	} */

	void TestMyStrcat()
	{

		const char source[30] = "Gaga"; 
		char dest[30] = "Lady"; 

		const char source2[30] = "Gaga"; 
		char dest2[30] = "Lady"; 
		
		strcat(dest, source);
		MyStrcat(dest2, source2);

		if (dest2 == source2)
		{
			printf("Mystrcat: PASS\n");
		}

		else
		{
			printf("Mystrcat: FAIL\n");
		}

		
	}
	
	void TestMyStrncat()
	{
		size_t num_of_bytes = 2;

		const char source[30] = "Gaga"; 
		char dest[30] = "Lady"; 

		const char source2[30] = "Gaga"; 
		char dest2[30] = "Lady"; 

		strncat(dest, source, num_of_bytes);		
		MyStrncat(dest2, source2, num_of_bytes);

		if (dest2 == source2)
		{
			printf("Mystrncat: PASS\n");
		}

		else
		{
			printf("Mystrncat: FAIL\n");
		}

	}
	

	size_t MyStrlen(const char *s)
	{
		size_t string_length = 0;
		assert(s);

		while ('\0' != *s)
		{
			string_length++;
			s++;
		}	 
		return string_length;
	}	

	
	int MyStrcmp(const char *str1, const char *str2)
	{
		assert(str1);
		assert(str2);

		while ('\0' != *str1 && '\0' != *str2)
		{		
			if (*str1 < *str2 || *str1 > *str2) 
			{
				return *str1 - *str2;
			}

			str1++;
			str2++;
		}
	
		return 0;
	}	

	char *MyStrcpy(char *dest, const char *source)
	{
		assert(dest);
		assert(source);

		while ('\0' != *source)
		{		
			*dest = *source;
			source++;
			dest++;
		}
		
		*dest = '\0';
	
		return dest;
	}


	char *MyStrncpy(char *dest, const char *source, size_t n)
	{
		size_t index;
		char *pointer_to_dest = dest; /*  pointer to the first element in dest */

		assert(dest);
		assert(source);

		for (index = 0; index < n && '\0' != *source; index++)
		{		
			*dest = *source;
			source++;
			dest++;
		}

		for (index = index ; index < n; index++)
		{
		*dest = '\0';
		dest++;
		}
	
		return pointer_to_dest;
	}



	int MyStrcasecmp(const char *str1, const char *str2)
	{
		assert(str1);
		assert(str2);

		while ('\0' != *str1 && '\0' != *str2)
		{		
			if ((toupper(*str1) < toupper(*str2)) ||
			   (toupper(*str1) > toupper(*str2))) 
			{
				return *str1 - *str2;
			}

			++str1;
			++str2;
		}
	
		return 0;
	}	

	char *MyStrchr(const char *str, int locate_char)
	{
		assert(str);

		while ('\0' != *str)
		{		
			if (locate_char == *str)
			{
				return (char*)(str);
			}
			++str;
		}

		if ('\0' == locate_char)
			{
				return (char*)(str);
			}

		return NULL;
	}	

	char *MyStrdup(const char *str)
	{

		char *duplicated = (char*)(malloc(sizeof(str)));
		char *pointer_to_duplicated;

		assert(str);

		if (duplicated == NULL)
		{
			return NULL;
		}
		
		pointer_to_duplicated = duplicated;

		while ('\0' != *str)
		{

			*pointer_to_duplicated = *str;	
			++str;
			++pointer_to_duplicated;
		 }
		*pointer_to_duplicated = '\0';

		return (char*)(duplicated);
		free(duplicated);
	}	


	char *MyStrcat(char *dest, const char *src)
	{

		char *pointer_to_src = (char*)(src);	
		
		assert(dest);
		assert(src);

		while ('\0' != *dest)
		{
			++dest;
		}	
		
		while ('\0' != *pointer_to_src)
		{		
			*dest = *pointer_to_src;			
			++pointer_to_src;
			++dest;
		}

		*dest = '\0'; /* End of new string */

		return (char*)(dest);
	}	


	char *MyStrncat(char *dest, const char *src, size_t n)
	{
		char *pointer_to_src = (char*)(src);
		size_t index;	

		assert(dest);
		assert(src);

		while ('\0' != *dest)
		{
			++dest;
		}	
		
		for (index = 0; (index < n) && ('\0' != *pointer_to_src); 
		++index, ++pointer_to_src, ++dest)
		{		
			*dest = *pointer_to_src;			
		}

		*dest = '\0'; /* End of new string */

		return (char*)(dest);
	}	



