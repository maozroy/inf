#include <stdlib.h>
#include "barr.h"
#define TEST_MACRO (string, index, result) printf("Test: %s no. %d")

#define RUN_TEST(TEST, NAME) (TEST) ?\
printf ("Test %s\t" KGRN "PASS" KNRM "\n", NAME) :\
printf("Test %s\t" KRED "FAIL" KNRM "\n", NAME)

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

int main()
{
	bitarray arr = 0;
	bitarray i = 0;
	char* temp_p = NULL;
	bitarray test = 0;
	unsigned char result = 0;
	temp_p = malloc(sizeof(char)*(8+1));
	*temp_p = 0;
	arr = BArrSetAll(arr);


	arr = BArrResetAll(arr);


	arr = BArrSetBit(arr, 10, 1);
	arr = BArrSetBit(arr, 0, 1);
	arr = BArrSetBit(arr, 1, 1);
	arr = BArrSetBit(arr, 63, 1);
	arr = 0;
	
	printf("\ncount on lut: %ld, ",BArrCountOnLUT(0));
	printf("\ncount on non: %ld, ",BArrCountOn(0));

	
	/*for(i = 0 ; i < 256 ; i++)
	{

		result = BArrMirror_char(test);
		printf("%d, ", result);
		test++;
	}*/

	test = 285349412;
	i = 285349412;
	printf("%s",BArrToString(temp_p ,test));
	test = BArrMirror(test);
	printf("%s",BArrToString(temp_p ,test));
	i = BArrMirrorLUT(i);
	printf("%s",BArrToString(temp_p ,i));	
	
	
	
	free(temp_p);


return 0;
}

