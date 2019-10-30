#include "barr.h"
#define TEST_MACRO (string, index, result) printf("Test: %s no. %d")

int main()
{
	bitarray arr = 0;
	bitarray i = 0;
	char* temp_p = NULL;
	bitarray test = 0;
	unsigned char result = 0;
	temp_p = malloc(sizeof(char)*(BITS+1));
	*temp_p = 0;
	arr = BArrSetAll(arr);


	arr = BArrResetAll(arr);


	arr = BArrSetBit(arr, 10, 1);
	arr = BArrSetBit(arr, 0, 1);
	arr = BArrSetBit(arr, 1, 1);
	arr = BArrSetBit(arr, 63, 1);
	arr = 0;

	
	/*for(i = 0 ; i < 256 ; i++)
	{

		result = BArrMirror_char(test);
		printf("%d, ", result);
		test++;
	}*/
	printf("%s",BArrToString(temp_p ,arr));
	test = 223;
	i = 223;
	test = BArrMirror(test);
	printf("%s",BArrToString(temp_p ,test));
	i = BArrMirrorLUT(i);
	printf("%s",BArrToString(temp_p ,i));	
	free(temp_p);


return 0;
}


