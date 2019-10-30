#include <stdio.h> /*prints*/
#include <string.h> /* memset */
#include <stdlib.h> /*mallocs and atoi*/
#include <math.h> /* POW */

/*personalized memset*/
void *MyMemset (void *str, int c, size_t n);

/*small function to loop through address and replace letter*/
void ByteMemset(char *ptr, int c, size_t n);

/*personalized memcpy*/
void *MyMemcpy(void *str1, void *str2, size_t n);

/*small function to loop through address 2 address and replace*/
void ByteMemcpy(char *ptr1, const char *ptr2, size_t n);
void WordMemcpy(size_t *ptr1, const size_t *ptr2, size_t n);

/*personalized memmove. will use memcpy if possible*/
void *MyMemmove (void *dst_p, void *src_p, unsigned int n);

/*prints chars that exist in 2 first arr and are not on third. using LUTable*/
void CheckThreeArrays (char* arr1, char* arr2, char* arr3);

/*prints if system is little endian or not*/
void IsLittleEndian();

/*turns num into an array for any base*/
char *MyItoa(int num, char* str, int base);

/*turns array into an int for any base*/
int  MyAtoi(char* str, size_t base);

void TestMyMemset();
void TestMyMemcpy();
void TestMyMemmove();
void TestItoaGeneral();
void TestAtoiGeneral();
void TestPrintLetters();
