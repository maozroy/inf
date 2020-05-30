
#include "hashtable.h"
#include <stdio.h>
#include <string.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

size_t hash_func(const void *key);
int is_match (const void *current_data, const void *data_to_compare);

void TestCreate(void);

int main()
{
	TestCreate();
	return 0;
}

void TestCreate(void)
{
 	char buff[1024] = {0};
 	hash_table_t * my_hash = NULL;
    FILE *test = fopen("test.txt", "r");
    char *string = fgets(buff, 1024, test);
    
   	my_hash = HashCreate(256, hash_func, is_match);
   	
   	string = strtok(buff, " ");
	while (string != NULL)
    {		
    		if (string != NULL)
    		{
          	 HashInsert(my_hash, string);
          	 }
    	    string = strtok(NULL ," ");



    }
    

	printf("size is %ld\n",HashSize(my_hash));
	printf("load is %f\n",HashGetLoadFactor(my_hash));
	printf("std is %f\n",HashGetStandardDeviation(my_hash));
	
    fclose(test);
                       
	HashDestroy(my_hash);
	
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
