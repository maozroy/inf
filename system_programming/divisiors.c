#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define THREAD_NUM 5
#define BIG_NUM 100

typedef struct numbers
{
	size_t min_range;
	size_t max_range;
	size_t total_num;
}num_struct;

num_struct result_arr[THREAD_NUM+1];
pthread_t thread_id[THREAD_NUM+1] = {0};

void *DiviseAndConquer(void *my_struct);



int main ()
{
	size_t i = 0;
	size_t current_num = 1;
	void **temp_val = malloc(sizeof(size_t));
	size_t result[THREAD_NUM + 1] = {0};
	size_t sum = 0;
	
	#pragma omp parallel for private(i)
	{
		result_arr[i].min_range = current_num;
		current_num += BIG_NUM / THREAD_NUM;
		result_arr[i].max_range = current_num;
		result_arr[i].total_num = BIG_NUM;
		
		result[i] = (size_t)DiviseAndConquer(&result_arr[i]);
	}
	
	if ((BIG_NUM % THREAD_NUM)!= 0)
	{
		printf("??");
		result[THREAD_NUM + 1] += BIG_NUM;
	}
	
	for (i = 0 ; i < THREAD_NUM+1 ; i++)
	{
		sum += result[i];
	}
	
	printf("\nresult is %ld", sum);
	return 0;
}

void *DiviseAndConquer(void *my_struct)
{
	size_t i = 0;
	size_t begin = ((num_struct *)my_struct) -> min_range;
	size_t end = ((num_struct *)my_struct) -> max_range;
	size_t total = ((num_struct *)my_struct) -> total_num;
	size_t sum = 0;
   	printf("begin is %ld\n",begin);
	
	for (i = begin ; i < end ; ++i)
	{
		if (0 == (total % i))
		{
			sum += i;
		}
	}
	((num_struct *)my_struct)->total_num = sum;
	return (void *)sum;

}

/*
	for (i = 0 ; i < THREAD_NUM; i++)
	{
		result_arr[i].min_range = current_num;
		current_num += BIG_NUM / THREAD_NUM;
		result_arr[i].max_range = current_num;
		result_arr[i].total_num = BIG_NUM;
		pthread_create(&thread_id[i], NULL, DiviseAndConquer, &result_arr[i]);
		result += (size_t)DiviseAndConquer(&result_arr[i]);
	}	*/
	
		/*
	for (i = 0 ; i < THREAD_NUM ; ++i)
	{
		/*pthread_join(thread_id[i], temp_val);*/
		/*result += *(size_t *)temp_val;
	}*/
