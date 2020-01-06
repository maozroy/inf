#include <stdio.h> /* printf */
#include <math.h> /* pow */
#include <string.h> /* memcmp */

#include "dhcp.h"
#include "../MyUtils.h" /* MAX2,MIN2 */

#define MASK1 24
#define MASK2 27
#define MASK3 30

void TestCreateDestroy();
void TestAlloc();
void TestFreeIP();
void TestTreeFull();

int main()
{
	TestCreateDestroy();
	TestAlloc();
	TestFreeIP();
	TestTreeFull();		

	return 0;
}

void TestCreateDestroy()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK1);
	
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}

void TestAlloc()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK2);
	size_t res = pow(2, 32 - MASK2) - 3;
	unsigned char result_ip[4] = {0};
	unsigned char valid_addr1[] = {192,168,0,15};
	unsigned char valid_addr2[] = {192,168,0,12};
	unsigned char valid_addr3[] = {192,168,0,3};
	unsigned char valid_addr4[] = {192,168,0,10};
	unsigned char invalid_addr1[] = {192,168,255,13};
	
	PRINTTESTRESULTS("TestAlloc_CountFree",1, res == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",2, 0 == DHCPAllocIP(new_dhcp, valid_addr1, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",3, 0 == memcmp(result_ip, valid_addr1, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestAlloc_CountFree",4, res - 1 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",5, 0 == DHCPAllocIP(new_dhcp, valid_addr2, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",6, 0 == memcmp(result_ip, valid_addr2, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestAlloc_CountFree",7, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",8, 0 == DHCPAllocIP(new_dhcp, valid_addr3, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",9, 0 == memcmp(result_ip, valid_addr3, IP_SIZE * sizeof(unsigned char)));	
	
	PRINTTESTRESULTS("TestAlloc_CountFree",10, res - 3 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",11, 0 == DHCPAllocIP(new_dhcp, valid_addr4, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",12, 0 == memcmp(result_ip, valid_addr4, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestAlloc_CountFree",13, res - 4 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",14, 2 == DHCPAllocIP(new_dhcp, invalid_addr1, result_ip));
	PRINTTESTRESULTS("TestAlloc_CountFree",15, res - 4 == DHCPCountFree(new_dhcp));
	
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}

void TestFreeIP()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK1);
	size_t res = pow(2, 32 - MASK1) - 3;
	unsigned char result_ip[4] = {0};
	unsigned char valid_addr1[] = {192,168,0,15};
	unsigned char valid_addr2[] = {192,168,0,12};
	unsigned char valid_addr3[] = {192,168,0,3};
	unsigned char valid_addr4[] = {192,168,0,10};
	unsigned char valid_addr5[] = {192,168,0,1};
	unsigned char default_addr1[] = {192,168,0,255};
	unsigned char default_addr2[] = {192,168,0,254};
	unsigned char default_addr3[] = {192,168,0,0};
	int res1 = 0;
	
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",1, res == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",2, 0 == DHCPAllocIP(new_dhcp, valid_addr1, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",3, 0 == memcmp(result_ip, valid_addr1, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",4, res - 1 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",5, 0 == DHCPAllocIP(new_dhcp, valid_addr2, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",6, 0 == memcmp(result_ip, valid_addr2, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",7, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",8, 0 == DHCPAllocIP(new_dhcp, valid_addr3, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",9, 0 == memcmp(result_ip, valid_addr3, IP_SIZE * sizeof(unsigned char)));	
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",10, res - 3 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",11, 0 == DHCPAllocIP(new_dhcp, valid_addr4, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",12, 0 == memcmp(result_ip, valid_addr4, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",13, res - 4 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",14, 0 == DHCPFreeIP(new_dhcp, valid_addr3));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",15, res - 3 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",16, 0 == DHCPFreeIP(new_dhcp, valid_addr1));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",17, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",18, 1 == DHCPFreeIP(new_dhcp, valid_addr3));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",19, res - 2 == DHCPCountFree(new_dhcp));
	res1 = DHCPFreeIP(new_dhcp, valid_addr5);
	printf("res1 = %d\n", res1);
	PRINTTESTRESULTS("TestFreeIP_Free",20, 1 == res1);
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",21, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",22, 0 == DHCPFreeIP(new_dhcp, default_addr1));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",23, res - 1 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",24, 0 == DHCPFreeIP(new_dhcp, default_addr2));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",25, res == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",26, 0 == DHCPFreeIP(new_dhcp, default_addr3));
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}

void TestTreeFull()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK3);
	size_t res = pow(2, 32 - MASK3) - 3;
	unsigned char result_ip[4] = {0};
	unsigned char ip_addr[] = {192,168,0,1};
	
	PRINTTESTRESULTS("TestTreeFull_CountFree",1, res == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestTreeFull_Alloc",2, 0 == DHCPAllocIP(new_dhcp, ip_addr, result_ip));
	PRINTTESTRESULTS("TestTreeFull_check_result_ip",3, 0 == memcmp(result_ip, ip_addr, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestAlloc_CountFree",4, res - 1 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestTreeFull_AllocFull",5, 3 == DHCPAllocIP(new_dhcp, ip_addr, result_ip));
	PRINTTESTRESULTS("TestAlloc_CountFree",6, res - 1 == DHCPCountFree(new_dhcp));
	
	PRINTTESTRESULTS("TestFreeIP_Free",7, 0 == DHCPFreeIP(new_dhcp, ip_addr));
	PRINTTESTRESULTS("TestAlloc_CountFree",8, res == DHCPCountFree(new_dhcp));
	
	PRINTTESTRESULTS("TestTreeFull_Alloc",9, 0 == DHCPAllocIP(new_dhcp, ip_addr, result_ip));
	PRINTTESTRESULTS("TestTreeFull_check_result_ip",10, 0 == memcmp(result_ip, ip_addr, IP_SIZE * sizeof(unsigned char)));
	PRINTTESTRESULTS("TestAlloc_CountFree",11, res - 1 == DHCPCountFree(new_dhcp));
	
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}
