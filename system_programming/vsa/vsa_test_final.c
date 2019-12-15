#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h> 
#include <sys/types.h>
#include "vsa.h"

#define PS printf("\33[0;32m"
#define PF printf("\33[0;31m"
#define PN printf("\33[0;33m"
#define EP0 "\n\33[0m");
#define EP1(a) "\n\33[0m", a);
#define EP2(a, b) "\n\33[0m", a, b);
#define EP3(a, b, c) "\n\33[0m", a, b, c);
#define EP4(a, b, c, d) "\n\33[0m", a, b, c, d);
#define EP5(a, b, c, d, e) "\n\33[0m", a, b, c, d, e);
#define EP6(a, b, c, d, e, f) "\n\33[0m", a, b, c, d, e, f);

#define RUN 100000
int error = 0;

void StressTheSystem();
void TestTheSystem();
void TestReadWrite();
void CheckEastFunc(int i);


int main()
{
	TestTheSystem(); /*simple test for debugging*/
	StressTheSystem(); /*stress test for the VSA, if you pass this, your VSA is pretty much guaranteed to work perfectly*/
	if (error)
	{
		PF"Oh no!, your program didnt crash, but you lost data somewhere!"EP0
	}
	else
	{
		PS"your allocator works as requested by the client!"EP0
	}


	return 0;
}

void StressTheSystem()
{
	size_t size_or_off_arr[RUN] = {0}, div_counter = 0;
	size_t size = RUN, available_size = 0;
	int start_size = 0, end_size = 0;
	int ashalf = 0;
	vsa_t *vsa = NULL;
	void *block[RUN] = {NULL};
	int r = rand() % 100;
	struct timeval time;
	int time1, i = 0, j = 0, q = 0;
	void *alloc = malloc(size);
	vsa = VSAInit(alloc, size);
	
	start_size = VSALargestChunk(vsa);

	gettimeofday(&time, NULL);
	srand(time.tv_sec);
	time1 = time.tv_sec + 3;
	while (time.tv_sec < time1 || div_counter < 3001)
	{
		div_counter++;
		gettimeofday(&time, NULL);
		i = 0;
		available_size = VSALargestChunk(vsa);
		while (available_size)
		{

			ashalf = available_size;
			ashalf /= 2;
			ashalf++;
			r = rand() % ashalf;
			r++;
			if(!size_or_off_arr[i])
			{
				block[i] = VSAAlloc(vsa, r);
				size_or_off_arr[i] = r;
/*				printf("allocating: %d = %lu at address: %p\n", i, size_or_off_arr[i], block[i]);
*/				available_size = VSALargestChunk(vsa);
/*				printf("available_size: %lu\n", available_size);
*/			}
			++i;
		}
		j = i;
		r = rand() % (i);
		while (j-5 && j)
		{
			while (!size_or_off_arr[r] && q < 10000)
			{
				r = rand() % (i);
				++q;
			}
			q = 0;
			VSAFree(block[r]);/*
			available_size = VSALargestChunk(vsa);
			printf("available_size: %lu\n", available_size);*/
			size_or_off_arr[r] = 0;
			--j;
		}
		i = 0;
		if (!(div_counter % 1000))
		{
			while (i<RUN)
			{
				if (size_or_off_arr[i])
				{
					VSAFree(block[i]);
					size_or_off_arr[i] = 0;
				}
				i++;
			}
		}
		i = 0;
	}
	TestReadWrite();
	i = 0;
	while (i<RUN)
	{
		if (size_or_off_arr[i])
		{
			VSAFree(block[i]);
			size_or_off_arr[i] = 0;
		}
		i++;
	}
	end_size = VSALargestChunk(vsa);
	error += (!(end_size == start_size));
	free(alloc);

}

void TestTheSystem()
{
	size_t size = 500;
	vsa_t *vsa = NULL;
	void *block[10] = {NULL};
	vsa = VSAInit(malloc(size), size);
	printf("initialized\n");
	printf("size: %lu\n", VSALargestChunk(vsa));
	block[0] = VSAAlloc(vsa, 15);
	printf("size: %lu\n", VSALargestChunk(vsa));
	block[1] = VSAAlloc(vsa, 264);
	printf("size: %lu\n", VSALargestChunk(vsa));
	block[2] = VSAAlloc(vsa, 21);
	printf("size: %lu\n", VSALargestChunk(vsa));
	block[3] = VSAAlloc(vsa, 17);
	printf("size: %lu\n", VSALargestChunk(vsa));
	block[4] = VSAAlloc(vsa, 52);
	printf("size: %lu\n", VSALargestChunk(vsa));
	VSAFree(block[4]);
	printf("size: %lu\n", VSALargestChunk(vsa));
	VSAFree(block[2]);
	printf("size: %lu\n", VSALargestChunk(vsa));
	VSAFree(block[0]);
	printf("size: %lu\n", VSALargestChunk(vsa));
	VSAFree(block[1]);
	printf("size: %lu\n", VSALargestChunk(vsa));
	VSAFree(block[3]);
	printf("size: %lu\n", VSALargestChunk(vsa));
	free(vsa);
}

void TestReadWrite()
{
	int counter = 0, i = 0;
	size_t size = 500;
	vsa_t *vsa = NULL;
	void *block[10] = {NULL};
	int div_counter = 1;
	vsa = VSAInit(malloc(size), size);
	block[0] = VSAAlloc(vsa, 15);
	block[1] = VSAAlloc(vsa, 264);
	block[2] = VSAAlloc(vsa, 21);
	block[3] = VSAAlloc(vsa, 17);
	block[4] = VSAAlloc(vsa, 52);
	VSAFree(block[4]);
	VSAFree(block[2]);
	VSAFree(block[0]);
	VSAFree(block[1]);
	VSAFree(block[3]);
	if (!size)
	{
		block[0] = VSAAlloc(vsa, 15);
		block[1] = VSAAlloc(vsa, 264);
		block[2] = VSAAlloc(vsa, 21);
		block[3] = VSAAlloc(vsa, 17);
		block[4] = VSAAlloc(vsa, 52);
		VSAFree(block[4]);
		VSAFree(block[2]);
		VSAFree(block[0]);
		VSAFree(block[1]);
		VSAFree(block[3]);
		block[0] = VSAAlloc(vsa, 15);
		block[1] = VSAAlloc(vsa, 264);
		block[2] = VSAAlloc(vsa, 21);
		block[3] = VSAAlloc(vsa, 17);
		block[4] = VSAAlloc(vsa, 52);
		VSAFree(block[4]);
		VSAFree(block[2]);
		VSAFree(block[0]);
		VSAFree(block[1]);
		VSAFree(block[3]);
	}
	while (counter < 5)
	{
		if ((div_counter % 1000))
		{
			while (i<RUN)
			{
				if (i)
				{
					CheckEastFunc(i);
				}
				i++;
			}
		}
		counter++;
	}
	if (counter == 5)
	{
		goto here;
	}
	block[0] = VSAAlloc(vsa, 15);
	block[1] = VSAAlloc(vsa, 264);
	block[2] = VSAAlloc(vsa, 21);
	block[3] = VSAAlloc(vsa, 17);
	block[4] = VSAAlloc(vsa, 52);
	here:
	sleep(2);
	printf("Segmentation fault (core dumped)\n");
	sleep(1);
	printf("Error of type: Unknown, seeking help, and calling the FBI!\n");
	sleep(3);
	printf("**** stack smashing detected ***: ./a.out terminated*\n"
		   "*======= Backtrace: =========*\n"
		   "*/lib/tls/i686/cmov/libc.so.6(__fortify_fail+0x48)Aborted*\n");
	sleep(3);
	printf("Floating point exception (core dumped)\n");
	sleep(4);
	printf("your code has been deleted and core was dumped again!\n");
	sleep(4);
	printf("seriously dude, press Ctrl + Alt + Delete, and restart your computer!\n");
	sleep(5);
	printf("just kidding!!! you program survived the test without crashing!!!\n\n\n\n\n");
	sleep(5);
	VSAFree(block[4]);
	VSAFree(block[2]);
	VSAFree(block[0]);
	VSAFree(block[1]);
	VSAFree(block[3]);
	free(vsa);
}

void CheckEastFunc(int i)
{
	int j = i + 5;
	i = j;
	j++;
}
