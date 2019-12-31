 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	Jonathan
*						SpellChecker					
*	Date: Thu Dec 26 16:27:29 IST 2019		                
*																				
********************************************/

#include <stdlib.h>   /* malloc*/
#include <stdio.h>    /* printf */
#include <string.h>   /* strtok */
#include <strings.h>  /* strcasecmp */
#include <ctype.h>    /* toupper */
#include <sys/stat.h> /* file size */
#include <sys/mman.h> /* mmap */
#include <fcntl.h>    /* open */
#include <unistd.h>   /* lseek */

#include "../dllist/dllist.h"
#include "../hashtable/hashtable.h"
#define DICT_LOCATION ("/usr/share/dict/american-english")
#define INPUT_CAP 256
#define NUM_OF_LETTERS 26

static void LoadDictIMP(char **ptr_to_free);
static void CreateHashIMP(hash_table_t **hash, char **dic_string);
static void GetUserStringIMP(char **user_string);
static size_t hash_funcIMP(const void *key);
static int is_matchIMP (const void *current_data, const void *data_to_compare);
static void ParseAndReturnNonMatchIMP(hash_table_t *hash, char **user_string);

int main()
{
	char *dic_string = NULL;
	char input_string[INPUT_CAP] = {0};
	hash_table_t *hash_table = NULL;
	char *ptr = input_string;
	
	LoadDictIMP(&dic_string);
	CreateHashIMP(&hash_table, &dic_string);
	GetUserStringIMP(&ptr);
	ParseAndReturnNonMatchIMP(hash_table ,&ptr);
	
	HashDestroy(hash_table);
	
	return 0;
}

static void GetUserStringIMP(char **user_string)
{
	printf("\nenter string up tp %d chars:\t", INPUT_CAP);
	scanf("%[^\n]s", *user_string);
}

static void ParseAndReturnNonMatchIMP(hash_table_t *hash, char **user_string)
{
	*user_string = strtok(*user_string, " ");
	
	while (NULL != *user_string)
	{
		if (NULL == HashFind(hash, *user_string))
		{
			printf("%s ", *user_string);
		}
		*user_string = strtok(NULL, " ");
	}
}

static void LoadDictIMP(char **dic_string)
{
	int fd = 0;
	int file_size = 0;
	
	fd = open(DICT_LOCATION, O_RDONLY, 0);
	if (fd < 0)
	{
		return;
	}
	
	file_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	
	*dic_string = mmap(NULL, file_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
}

static void CreateHashIMP(hash_table_t **hash, char **dic_string)
{
	char *dic_runner = NULL;
	*hash = HashCreate(NUM_OF_LETTERS + 2, hash_funcIMP, is_matchIMP);
	dic_runner = *dic_string;
	dic_runner = strtok(dic_runner, "\n");
	
	while (NULL != dic_runner )
	{
		HashInsert(*hash, dic_runner);
		dic_runner = strtok(NULL, "\n");
	}
}

static size_t hash_funcIMP(const void *key)
{
	char *data = (void *)key;
	size_t bucket = (size_t) toupper(*data) - 'A';

	if (bucket > NUM_OF_LETTERS)
	{
		return NUM_OF_LETTERS + 1;
	}
	
	return bucket;
}

static int is_matchIMP (const void *current_data, const void *data_to_compare)
{
	return (0 == strcasecmp((char *)current_data, (char *)data_to_compare));
}
