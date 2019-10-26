#include <stdio.h> 
#include <string.h> 
#define WORD sizeof(size_t)

void byte_memset(char *ptr, int c, size_t n);
void byte_memcpy(char *ptr1, const char *ptr2, size_t n);
void WORD_memcpy(size_t *ptr1, const size_t *ptr2, size_t n);
void reverse_byte_memcpy(char *ptr1, const char *ptr2, size_t n);
void reverse_WORD_memcpy(size_t *ptr1, const size_t *ptr2, size_t n);
void *mymemset (void *str, int c, size_t n);
void *mymemcpy(void *str1, void *str2, size_t n);
void *mymemmove (void *str1, void *str2, int n);

void *mymemcpy(void *str1, void *str2, size_t n)
{
	size_t *st1_p = str1;
	size_t *st2_p = str2;
	char *ch1_p = str1;
	char *ch2_p = str2;
	unsigned int end_loop = 0;
	unsigned int start_loop = WORD - ( (*(size_t*) & st1_p) % WORD);
	unsigned int chunk_loop = 0;
	
	if (8 == start_loop)
	{
		start_loop = 0;
	}
	/*check if n is too large*/
	if (strlen(ch1_p) < n)
	{
		n = strlen(ch1_p);
	}
	
	chunk_loop = (n - start_loop) / WORD;
	/*start chunk loop and move pointers to the right position*/
	byte_memcpy(ch1_p, ch2_p, start_loop); /*start tail loop*/
	ch1_p = ch1_p+start_loop;
	ch2_p = ch2_p+start_loop;
	st1_p =(size_t*) ch1_p;
	st2_p =(size_t*) ch2_p;
	
	/*start chunk loop and move pointers to the right position*/
	WORD_memcpy(st1_p, st2_p, chunk_loop); 
	st1_p = st1_p+chunk_loop;
	st2_p = st2_p+chunk_loop;
	ch1_p = (char*)st1_p;
	ch2_p = (char*)st2_p;
	end_loop = n - chunk_loop*(WORD) - start_loop;
	
	byte_memcpy(ch1_p, ch2_p, end_loop);/*end tail loop*/
	
	return str1;
}



void *mymemmove (void *str1, void *str2, int n)
{
	size_t str1_num = (*(size_t*) & str1);
	size_t str2_num = (*(size_t*) & str2);
	
	size_t *st1_p = (size_t*)str1+(n/WORD);
	size_t *st2_p = (size_t*)str2+(n/WORD);
	char *ch1_p = (char*)str1+n-1;
	char *ch2_p = (char*)str2+n-1;
	unsigned int chunk_loop = n % WORD;
	unsigned int start_loop = n - (chunk_loop)*WORD;

	
	if ( !(( str1_num >  str2_num ) && ( str1_num <= str2_num+n)) )
	{
		printf("use my memcpy!\n");
		/*return;*/
	}
	/*start chunk loop and move pointers to the right position*/
	reverse_byte_memcpy(ch1_p, ch2_p, start_loop); /*start tail loop*/
	ch1_p = ch1_p-start_loop;
	ch2_p = ch2_p-start_loop;
	st1_p =(size_t*) ch1_p;
	st2_p =(size_t*) ch2_p;
	
	/*start chunk loop and move pointers to the right position*/
	reverse_WORD_memcpy(st1_p, st2_p, chunk_loop); 
	st1_p = st1_p-chunk_loop;
	st2_p = st2_p-chunk_loop;
	ch1_p = (char*)st1_p;
	ch2_p = (char*)st2_p;
	
	
	return str1;


}




void *mymemset (void *str, int c, size_t n)
{
	unsigned int i = 0;
	size_t *st_p = str;
	char *ch_p = str;
	size_t insert_to_string = 0;
	unsigned int end_loop = 0;
	unsigned int start_loop = WORD - ( (*(size_t*) & st_p) % WORD);
	unsigned int chunk_loop = 0;
	
	if (WORD == start_loop)
	{
		start_loop = 0;
	}
	/*check if n is too large*/
	if (strlen(ch_p) < n)
	{
		n = strlen(ch_p);
	}
	
	chunk_loop = (n - start_loop) / WORD;
	/*define block of c*/
	for (; i < WORD ; i++)
	{

		insert_to_string <<= WORD;
		insert_to_string |= c;
	}
	
	byte_memset(ch_p, c, start_loop); /*start tail loop*/
	ch_p = ch_p+start_loop;
	st_p =(size_t*) ch_p;
	
	for (i = 0; i < chunk_loop ; i++)
	{
		*(st_p) = insert_to_string;
		++st_p;
	}
	
	ch_p = (char*)st_p;
	end_loop = n - chunk_loop*(WORD) - start_loop;
	byte_memset(ch_p, c, end_loop); /*end tail loop*/
	
	return str;
}

void byte_memset(char *ptr, int c, size_t n)
{
	unsigned int i;
	
	for(i = 0 ; i < n ; i++)
	{
		*ptr = c;
		++ptr;
	}
}



void byte_memcpy(char *ptr1, const char *ptr2, size_t n)
{
	unsigned int i;
	
	for(i = 0 ; i < n ; i++)
	{
		*ptr1 = *ptr2;
		++ptr1;
		++ptr2;
	}
}

void reverse_byte_memcpy(char *ptr1, const char *ptr2, size_t n)
{
	unsigned int i;
	
	for(i = 0 ; i < n ; i++)
	{
		*ptr1 = *ptr2;
		--ptr1;
		--ptr2;

	}
}

void WORD_memcpy(size_t *ptr1, const size_t *ptr2, size_t n)
{
	unsigned int i;
	
	for(i = 0 ; i < n ; i++)
	{
		*ptr1 = *ptr2;
		++ptr1;
		++ptr2;
	}
}

void reverse_WORD_memcpy(size_t *ptr1, const size_t *ptr2, size_t n)
{
	unsigned int i;
	
	for(i = 0 ; i < n ; i++)
	{

		--ptr1;
		--ptr2;
		*ptr1 = *ptr2;
	}
}
