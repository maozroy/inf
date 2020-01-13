#include <pthread.h>
#include <stdlib.h>


#define SIZE 100000

int arr[SIZE];
pthread_t thread_id[SIZE] = {0};

void *AddToArr(void *index);

int main()
{
	int i = 0;
	int status = 1;
	pthread_attr_t attr = {0};
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	for (i = 0 ; i < SIZE ; i++)
	{
		while (status != 0)
		{
			status = pthread_create(&thread_id[i], &attr, AddToArr, (void *)i);
		}
		status = 1;
		
		/*pthread_detach(thread_id[i]);*/

	}
	/*
	for (i = 0 ; i < SIZE ; i++)
	{
		pthread_detach(thread_id[i]);

	}
*/
	for (i = 0 ; i < SIZE ; i++)
	{
		if (i != arr[i])
		{
			printf("failed at %d", i);
			return 0;
		}
		printf("%d is ok \n",i);
	}

	return 0;
}




void *AddToArr(void *index)
{
	int i = (int)index;
	
	arr[i] = i;
	
	return NULL;
}
