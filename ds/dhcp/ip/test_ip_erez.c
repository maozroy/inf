/****************************************************************************
*                                                                           *
*                               OL79 - 17/11/19                             *
*                               Auther - Erez                               *	
*                               Uniqe Identifier Def                        *
*                               Reviewer -??? 	                            *
*****************************************************************************/

#include <string.h> /* memset */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <unistd.h> /* getpid */

#include "ip.h"
#include "../my_utils.h"

#define BYTE_SIZE (8)

void TestIPtoBIT();
void TestBitToIp();
void TestCutSubnet();
void TestAddSubnet();
void TestIsValid();

/****************************************************/
int main()
{
	TestIPtoBIT();
	TestBitToIp();
	TestCutSubnet();
	TestAddSubnet();
	TestIsValid();

	return (0);
}
/************************/
void TestIPtoBIT()
{
	IPAddress ip_add1 = {'a','b','c','d'};
	BinaryIp ip_bin1 = 0X61626364;
	IPAddress ip_add2 = {' ','~','(',')'};
	BinaryIp ip_bin2 = 0X207E2829;
	IPAddress ip_add3 = {'@','$','&','*'};
	BinaryIp ip_bin3 = 0X4024262A;

	int test = 1;

	printf("********** Test IP To Bit **********\n\n");

	(ip_bin1 == IPtoBit(ip_add1)) ? (test *= 1) : (test = 0);
	(ip_bin2 == IPtoBit(ip_add2)) ? (test *= 1) : (test = 0);
	(ip_bin3 == IPtoBit(ip_add3)) ? (test *= 1) : (test = 0);

	RUN_TEST(test," IP To Bit Test");
}
/************************/
void TestBitToIp()
{
	IPAddress ip_add1 = {'a','b','c','d'};
	BinaryIp ip_bin1 = 0X61626364;
	IPAddress ip_add2 = {' ','~','(',')'};
	BinaryIp ip_bin2 = 0X207E2829;
	IPAddress ip_add3 = {'@','$','&','*'};
	BinaryIp ip_bin3 = 0X4024262A;
	IPAddress ip_test = {'-','-','-','-'};
	int i = 0;
	int test = 1;

	printf("********** Test Bit To IP **********\n\n");

	BitToIp(ip_bin1, ip_test);
	for (i = 0; i < IP_SIZE; ++i)
	{
		if (ip_add1[i] != ip_test[i])
		{
			test = 0;
		}
	}
	BitToIp(ip_bin2, ip_test);
	for (i = 0; i < IP_SIZE; ++i)
	{
		if (ip_add2[i] != ip_test[i])
		{
			test = 0;
		}
	}
	BitToIp(ip_bin3, ip_test);
	for (i = 0; i < IP_SIZE; ++i)
	{
		if (ip_add3[i] != ip_test[i])
		{
			test = 0;
		}
	}

	RUN_TEST(test," Bit To IP Test");
}
/************************/
void TestCutSubnet()
{
	IPAddress ip_add1 = {'a','b','c','d'};
	IPAddress ip_test = {'-','-','-','-'};
	BinaryIp ip_bin1 = 0X61626360;
	BinaryIp ip_bin2 = 0X61626300;
	BinaryIp ip_bin3 = 0X61626000;
	BinaryIp ip_bin4 = 0X61620000;	
	size_t mask = 0;
	int test = 1;
	int i = 0;

	printf("********** Test Cut Subnet **********\n\n");

	for (i = 0; i < IP_SIZE; ++i)
	{
		ip_test[i] = ip_add1[i];
	}
	mask = 28;
	CutSubnet(ip_test, mask);
	(ip_bin1 == IPtoBit(ip_test)) ? (test *= 1) : (test = 0);

/*	printf("Org: %x Mask: %d Outcome: %x\n",IPtoBit(ip_add1), mask, IPtoBit(ip_test));
*/
	for (i = 0; i < IP_SIZE; ++i)
	{
		ip_test[i] = ip_add1[i];
	}
	mask = 24;
	CutSubnet(ip_test, mask);
	(ip_bin2 == IPtoBit(ip_test)) ? (test *= 1) : (test = 0);

/*	printf("Org: %x Mask: %d Outcome: %x\n",IPtoBit(ip_add1), mask, IPtoBit(ip_test));
*/
	for (i = 0; i < IP_SIZE; ++i)
	{
		ip_test[i] = ip_add1[i];
	}
	mask = 20;
	CutSubnet(ip_test, mask);
	(ip_bin3 == IPtoBit(ip_test)) ? (test *= 1) : (test = 0);

/*	printf("Org: %x Mask: %d Outcome: %x\n",IPtoBit(ip_add1), mask, IPtoBit(ip_test));
*/
	for (i = 0; i < IP_SIZE; ++i)
	{
		ip_test[i] = ip_add1[i];
	}
	mask = 16;
	CutSubnet(ip_test, mask);
	(ip_bin4 == IPtoBit(ip_test)) ? (test *= 1) : (test = 0);

/*	printf("Org: %x Mask: %d Outcome: %x\n",IPtoBit(ip_add1), mask, IPtoBit(ip_test));
*/

	RUN_TEST(test," IP Cut Subnet");
}
/************************/
void TestAddSubnet()
{
	IPAddress ip_add1 = {'a','b','c','d'};
	IPAddress ip_test = {'-','-','-','-'};
	IPAddress ip_test1 = {'-','-','-','-'};
	BinaryIp ip_bin1 = 0X61626360;
	BinaryIp ip_bin2 = 0X61626300;
	BinaryIp ip_bin3 = 0X61626000;
	BinaryIp ip_bin4 = 0X61620000;	
	BinaryIp ip_bin11 = 0X4;
	BinaryIp ip_bin12 = 0X64;
	BinaryIp ip_bin13 = 0X364;
	BinaryIp ip_bin14 = 0X6364;
	size_t mask = 0;
	int test = 1;
	int i = 0;

	printf("********** Test Add Subnet **********\n\n");

	mask = 28;
	BitToIp(ip_bin1,ip_test1);
	AddSubnet(ip_test1, mask, ip_bin11, ip_test);
	for (i = 0; i < IP_SIZE; ++i)
	{
		if (ip_add1[i] != ip_test[i])
		{
			test = 0;
		}
	}

	mask = 24;
	BitToIp(ip_bin2,ip_test1);
	AddSubnet(ip_test1, mask, ip_bin12, ip_test);
	for (i = 0; i < IP_SIZE; ++i)
	{
		if (ip_add1[i] != ip_test[i])
		{
			test = 0;
		}
	}

	mask = 20;
	BitToIp(ip_bin3,ip_test1);
	AddSubnet(ip_test1, mask, ip_bin13, ip_test);
	for (i = 0; i < IP_SIZE; ++i)
	{
		if (ip_add1[i] != ip_test[i])
		{
			test = 0;
		}
	}

	mask = 16;
	BitToIp(ip_bin4,ip_test1);
	AddSubnet(ip_test1, mask, ip_bin14, ip_test);
	for (i = 0; i < IP_SIZE; ++i)
	{
		if (ip_add1[i] != ip_test[i])
		{
			test = 0;
		}
	}
	RUN_TEST(test," IP Cut Subnet");
}
/************************/
void TestIsValid()
{
	IPAddress ip_source = {'a','b','c','d'};
	IPAddress ip_add1 = {'a','b','c','d'};
	IPAddress ip_add2 = {'a','b','c','d'};
	size_t mask = 0;
	int test = 1;
	int i = 0;

	printf("********** Test Is Valid **********\n\n");

	mask = 28;
	(1 == IsValid(ip_add1, ip_add2, mask)) ? (test *= 1) : (test = 0);

	CutSubnet(ip_add2, mask);
	(1 == IsValid(ip_add1, ip_add2, mask)) ? (test *= 1) : (test = 0);

	mask = 30;
	(0 == IsValid(ip_add1, ip_add2, mask)) ? (test *= 1) : (test = 0);

	mask = 26;
	(1 == IsValid(ip_add1, ip_add2, mask)) ? (test *= 1) : (test = 0);

	for (i = 0; i < IP_SIZE; ++i)
	{
		ip_add1[i] = ip_source[i];
		ip_add2[i] = ip_source[i];
	}

	mask = 30;
	(1 == IsValid(ip_add1, ip_add2, mask)) ? (test *= 1) : (test = 0);

	RUN_TEST(test," Is Valid Test");
}
/************************/






