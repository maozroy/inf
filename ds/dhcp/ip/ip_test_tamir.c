/****************************************************************************
*                                                                           *
*                                Tamir Dayan                                *
*                              OL79 - 00/00/19                              *
*                                                                           *	
*                                Subject                                    *
*                           Reviewer:                                       *
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcmp */

#include "ip.h" /* IP */ 
#include "../myutils.h"

enum is_valid
{
	INVALID = 0,
	VALID = 1
};

void PrintBits(BinaryIp address);

void TestIPtoBit();
void TestBitToIp();
void TestCutSubnet();
void TestAddSubnet();
void TestIsValid();

int main()
{
	TestIPtoBit();
	TestBitToIp();
	TestCutSubnet();
/*	TestAddSubnet(); */
	TestIsValid();

	return 0;
}


void TestIPtoBit()
{
	IPAddress address = {10, 0, 0, 1};
	IPAddress address2 = {168, 12, 3, 2};

	printf("\n*****" BBLU "TestIPtoBit" KNRM "*****\n");
	TEST("TestIPtoBit", 1, 167772161 == IPtoBit(address));
	TEST("TestIPtoBit", 2, 2819359490 == IPtoBit(address2));
	printf("***** ********* *****\n");
}

void TestBitToIp()
{
	BinaryIp address = 167772161;
	BinaryIp address2 = 2819359490;
	IPAddress result_address = {0, 0, 0, 0};
	IPAddress result_address2 = {0, 0, 0, 0};

	printf("\n*****" BBLU "TestBitToIp" KNRM "*****\n");
	BitToIp(address, result_address);
	TEST("TestBitToIp", 1, 10 == result_address[0]);
	TEST("TestBitToIp", 2, 0 == result_address[1]);
	TEST("TestBitToIp", 3, 0 == result_address[2]);
	TEST("TestBitToIp", 4, 1 == result_address[3]);

	BitToIp(address2, result_address2);
	TEST("TestBitToIp", 5, 10 == result_address[0]);
	TEST("TestBitToIp", 6, 0 == result_address[1]);
	TEST("TestBitToIp", 7, 0 == result_address[2]);
	TEST("TestBitToIp", 8, 1 == result_address[3]);
	printf("***** ********* *****\n");
}

void TestCutSubnet()
{
	IPAddress address = {172, 16, 26, 13};
	IPAddress address2 = {10, 0, 0, 1};
	IPAddress address3 = {168, 12, 3, 2};
	IPAddress address4 = {168, 12, 3, 2};
	IPAddress address5 = {0, 0, 26, 13};


	printf("\n*****" BBLU "TestCutSubnet" KNRM "*****\n");
	CutSubnet(address, 16);
	TEST("TestCutSubnet", 1, 6669 == IPtoBit(address));
	TEST("TestCutSubnet", 2, 0 == memcmp(address, address5, 4));
	CutSubnet(address2, 32);
	TEST("TestCutSubnet", 3, 167772161 == IPtoBit(address2));
	CutSubnet(address3, 24);
	TEST("TestCutSubnet", 4, 2 == IPtoBit(address3));
	CutSubnet(address4, 32);
	TEST("TestCutSubnet", 5, 2819359490 == IPtoBit(address4));
	printf("***** ********* *****\n");
}
void TestIsValid()
{
	IPAddress address = {172, 16, 26, 13};
	IPAddress subnet = {172, 16, 0, 0};

	IPAddress address2 = {192, 168, 255, 254};
	IPAddress subnet2 = {192, 169, 255, 0};

	IPAddress address3 = {192, 168, 0, 125};
	IPAddress subnet3= {192, 168, 0, 0};

	printf("\n*****" BBLU "TestIsValid" KNRM "*****\n");

	TEST("TestIsValid", 1, VALID == IsValid(subnet, address, 24));
	TEST("TestIsValid", 2, INVALID == IsValid(subnet2, address2, 24));
	TEST("TestIsValid", 3, VALID == IsValid(subnet3, address3, 25));
	printf("***** ********* *****\n");
}

void PrintBits(BinaryIp address)
{
    size_t digit = 0;
    size_t temp = address;
    size_t i = 0;

    for (i = 0; i < 32; ++i)
    {   
        digit = temp & 1;
        printf("%ld",digit);
        temp = temp >> 1;
    }
    printf("\n");
}
