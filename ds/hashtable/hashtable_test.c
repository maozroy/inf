
#include "hashtable.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <errno.h>
#define DICT_LOC "/usr/share/dict/american-english"
#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

size_t hash_func(const void *key);
int is_match (const void *current_data, const void *data_to_compare);
int PrintAll(void *data, void *action_param);

void TestCreate(void);

int main()
{
	TestCreate();
	return 0;
}

void TestCreate(void)
{
 	int fd = 0;
 	char *string = NULL;
 	hash_table_t * my_hash = NULL;
    fd = open(DICT_LOC, O_RDWR,0777);
    
    if (fd < 0)
    {
		fprintf(stderr, "Failed to open: %s\n", strerror(errno));
		return;
	}
 /*   char *string = fgets(buff, 1024, fd);*/
    
   	my_hash = HashCreate(256, hash_func, is_match);
  	string = mmap (NULL, 100000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
   	string = strtok(NULL, "\n");
	while (string != NULL)
    {		
    		if (string != NULL)
    		{
          	 HashInsert(my_hash, string);
          	 }
    	    string = strtok(NULL ,"\n");



    }
	HashForEach(my_hash, PrintAll, NULL);

	printf("size is %ld\n",HashSize(my_hash));
	printf("load is %f\n",HashGetLoadFactor(my_hash));
	printf("std is %f\n",HashGetStandardDeviation(my_hash));
	

                       
	HashDestroy(my_hash);
	
}
int PrintAll(void *data, void *action_param)
{
	printf("%s\n", (char *)data);
	return 0;
}


size_t hash_func(const void *key)
{
	char *data = (void *)key;
	size_t bucket = *data;
	
	return bucket;
}

int is_match (const void *current_data, const void *data_to_compare)
{
	
	if (0 == strcmp(current_data, data_to_compare))
	{
		return 1;
	}
	return 0;
}
/*
typedef int (*action_func_t)(void *data, void *action_param);
typedef int (*is_match_t)(const void *current_data,
                          const void *data_to_compare);*/
