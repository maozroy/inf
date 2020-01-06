#include <stdio.h> /* printf */
#include <string.h>

#include "ip/ip.h"
#include "trie/trie.h"
#include "dhcp.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
#define UNUSED(x) ((void)(x))

void TestIsValid();
void TestIPToBitAndBitToIP();
void TestCutSubnet();
void TestTrieCreate();
void TestTrieInsert();
void TestTrieDeallocate();
void TestDHCPCreate();
void TestDHCPAllocIP1();
void TestDHCPAllocIP2();
void TestDHCPFreeIP();
void TestAddSubnet();

int main()
{
    TestIsValid();
    TestIPToBitAndBitToIP();
    TestCutSubnet();
    TestAddSubnet();
    TestTrieCreate();
    TestTrieInsert();
    TestTrieDeallocate();
    TestDHCPCreate();
    TestDHCPFreeIP();
    TestDHCPAllocIP1();
    TestDHCPAllocIP2();
    
    return 0;
}

void TestIsValid()
{
    int result = 0;     
    IPAddress address1 ={109,65,207,116};
    IPAddress address2 ={109,65,237,116}; 
    IPAddress subnet ={109,65,207,0}; 
    size_t mask = 24;

    printf("TestIsValid\n");
    result = IsValid(subnet, address1, mask);
    TEST1(result, 1);
    result = IsValid(subnet, address2, mask);
    TEST1(result, 0);
}

void TestIPToBitAndBitToIP()
{
    IPAddress address1 = {60,65,20,16};
    unsigned int bit_address = IPtoBit(address1);
    IPAddress result_ip = {0};
    BitToIp(bit_address, result_ip);
    
    printf("TestIPToBitAndBitToIP\n");
    TEST1(memcmp(address1, result_ip, 4), 0);
}

void TestCutSubnet()
{
   IPAddress address1 = {60,65,20,16};
   IPAddress address2 = {0,0,0,16};
   printf("TestCutSubnet\n");
   CutSubnet(address1, 24);
   TEST1(memcmp(address1, address2, 4), 0);
}

void TestAddSubnet()
{
    IPAddress address1 = {60,65,20,16};
    IPAddress address2 = {60,65,20,0};
    IPAddress address3 = {60,65,20,16};
    IPAddress address4 = {60,65,20,255};
    IPAddress result = {0};
    
    printf("TestAddSubnet\n");
    AddSubnet(address1, 24, 0, result); 
    TEST1(memcmp(result, address2, 4), 0);
    AddSubnet(address3, 24, 255, result); 
    TEST1(memcmp(result, address4, 4), 0);
}
void TestTrieCreate()
{
    trie_t *new_trie = TrieCreate(5);
    printf("TestTrieCreate\n");
    TEST2(new_trie);
    TrieDestroy(new_trie);
}

void TestTrieInsert()
{
    trie_t *trie = TrieCreate(5);
    IPAddress address1 = {60,65,20,5};
    IPAddress address2 = {60,65,20,4};
    IPAddress address3 = {60,65,20,1};
    IPAddress address4 = {60,65,20,1};
    IPAddress address5 = {60,65,20,10};
    unsigned int bit_address1 = IPtoBit(address1);
    unsigned int bit_address2 = IPtoBit(address2);
    unsigned int bit_address3 = IPtoBit(address3);
    unsigned int bit_address4 = IPtoBit(address4);
    unsigned int bit_address5 = IPtoBit(address5);
    trie_alloc_status_t status = T_SUCCESS_ALLOCATED_REQUESTED;
    
    printf("TestTrieInsert\n");
    status = TrieInsert(trie, bit_address1);
    TEST1(status, T_SUCCESS_ALLOCATED_REQUESTED);
    status = TrieInsert(trie, bit_address2);
    TEST1(status, T_SUCCESS_ALLOCATED_REQUESTED);
    status = TrieInsert(trie, bit_address3);
    TEST1(status, T_SUCCESS_ALLOCATED_REQUESTED);
    status = TrieInsert(trie, bit_address4);
    TEST1(status, T_REQUESTED_IP_OCCUPIED);
    TEST1(3, TrieCountAlloc(trie));
    status = TrieInsert(trie, bit_address5);
    TEST1(status, T_SUCCESS_ALLOCATED_REQUESTED);
    TEST1(4, TrieCountAlloc(trie)); 
    TEST1(28, TrieCountFree(trie)); 
    TrieDestroy(trie); 
}

void TestTrieDeallocate()
{
    trie_t *trie = TrieCreate(5);
    IPAddress address1 = {60,65,20,5};
    IPAddress address2 = {60,65,20,4};
    IPAddress address3 = {60,65,20,1};
    unsigned int bit_address1 = IPtoBit(address1);
    unsigned int bit_address2 = IPtoBit(address2);
    unsigned int bit_address3 = IPtoBit(address3);
    trie_free_status_t status = T_SUCCESS;
    
    printf("TestTrieDeallocate\n");
    TrieInsert(trie, bit_address1);
    TrieInsert(trie, bit_address2);
    TrieInsert(trie, bit_address3);
    TEST1(3, TrieCountAlloc(trie)); 
    status = TrieDeallocate(trie, bit_address1);
    TEST1(2, TrieCountAlloc(trie));
    TEST1(status, T_SUCCESS);
    status = TrieDeallocate(trie, bit_address1);
    TEST1(2, TrieCountAlloc(trie));
    TEST1(status, T_DOUBLE_FREE); 
    TrieDeallocate(trie, bit_address2);
    TrieDeallocate(trie, bit_address3);
    TEST1(0, TrieCountAlloc(trie));
    TEST1(32, TrieCountFree(trie)); 
    TrieDestroy(trie); 
}

void TestDHCPCreate()
{
    IPAddress subnet ={109,65,207,34}; 
    dhcp_t *dhcp = DHCPCreate(subnet, 24);

    printf("TestDHCPCreate\n");
    TEST2(dhcp);
    TEST1(253, DHCPCountFree(dhcp)); 
    DHCPDestroy(dhcp);
}


void TestDHCPAllocIP1()
{
    IPAddress subnet ={109,65,207,34};
    dhcp_t *dhcp = DHCPCreate(subnet, 20); 
    IPAddress address1 ={109,65,206,67};
    IPAddress address2 ={109,65,200,65};
    IPAddress address3 ={105,65,237,116};
    IPAddress address4 ={109,65,206,67};
    IPAddress address5 ={109,65,192,1};
    IPAddress address6 ={109,65,222,0};
    IPAddress buffer ={0};
    alloc_status_t status = 0;

    printf("TestDHCPAllocIP\n");
    status = DHCPAllocIP(dhcp, address1, buffer);
    TEST1(status, SUCCESS_ALLOCATED_REQUESTED);
    TEST1(0, memcmp(buffer, address1, 4));
    TEST1(4092, DHCPCountFree(dhcp));
    status = DHCPAllocIP(dhcp, address2, buffer);
    TEST1(status, SUCCESS_ALLOCATED_REQUESTED);
    TEST1(0, memcmp(buffer, address2, 4));
    TEST1(4091, DHCPCountFree(dhcp));
    status = DHCPAllocIP(dhcp, address3, buffer);
    TEST1(status, INVALID_IP);
    TEST1(4091, DHCPCountFree(dhcp));
    status = DHCPAllocIP(dhcp, address4, buffer);
    TEST1(status, SUCCESS_OTHER_IP);
    TEST1(0, memcmp(buffer, address5, 4));
    TEST1(4090, DHCPCountFree(dhcp));
    status = DHCPAllocIP(dhcp, address6, buffer);
    TEST1(status, INVALID_IP);

    DHCPDestroy(dhcp);
}

void TestDHCPFreeIP()
{
    IPAddress subnet ={109,65,207,34}; 
    dhcp_t *dhcp = DHCPCreate(subnet, 20);
    IPAddress address1 ={109,65,206,67};
    IPAddress address2 ={109,65,200,65};
    IPAddress address3 ={109,65,198,124};
    IPAddress address4 ={109,65,207,250};
    IPAddress address5 ={109,65,207,1};
    IPAddress address6 ={109,65,192,90};
    IPAddress address7 ={0,0,0,0};
    IPAddress address8 ={109,65,192,1};
    IPAddress address9 ={109,65,1,1};
    IPAddress address10 ={109,65,192,12};
    IPAddress buffer = {0};
    alloc_status_t status = 0;
    free_status_t free_status = 0;

    printf("TestDHCPFreeIP\n");
    DHCPAllocIP(dhcp, address1, buffer);
    DHCPAllocIP(dhcp, address2, buffer);
    DHCPAllocIP(dhcp, address3, buffer);
    DHCPAllocIP(dhcp, address4, buffer);
    DHCPAllocIP(dhcp, address5, buffer);
    DHCPAllocIP(dhcp, address6, buffer);
    TEST1(4087, DHCPCountFree(dhcp));
    free_status = DHCPFreeIP(dhcp, address1);
    TEST1(free_status, SUCCESS);
    TEST1(4088, DHCPCountFree(dhcp));
    DHCPFreeIP(dhcp, address2);
    DHCPFreeIP(dhcp, address3);
    DHCPFreeIP(dhcp, address4);
    DHCPFreeIP(dhcp, address5);
    DHCPFreeIP(dhcp, address6);
    free_status = DHCPFreeIP(dhcp, address6);
    TEST1(free_status, DOUBLE_FREE);
    TEST1(4093, DHCPCountFree(dhcp));
    status = DHCPAllocIP(dhcp, address7, buffer);
    TEST1(status, SUCCESS_OTHER_IP);
    TEST1(0, memcmp(buffer, address8, 4));
    free_status = DHCPFreeIP(dhcp, address9);
    TEST1(free_status, INVALID_SUBNET);
    free_status = DHCPFreeIP(dhcp, address10);
    TEST1(free_status, IP_NOT_FOUND);
    
    DHCPDestroy(dhcp);
}

void TestDHCPAllocIP2()
{
    IPAddress subnet ={109,65,207,1}; 
    dhcp_t *dhcp = DHCPCreate(subnet, 29);
    IPAddress address0 ={109,65,207,0};
    IPAddress address1 ={109,65,207,1};
    IPAddress address2 ={109,65,207,2};
    IPAddress address3 ={109,65,207,3};
    IPAddress address4 ={109,65,207,4};
    IPAddress address5 ={109,65,207,5};
    IPAddress address6 ={109,65,207,6};
    IPAddress address7 ={109,65,207,7};
    IPAddress buffer = {0};
    alloc_status_t status = 0;
    
    printf("TestDHCPAllocIP2\n");
    status = DHCPAllocIP(dhcp, address0, buffer);
    TEST1(status, SUCCESS_OTHER_IP);
    TEST1(0, memcmp(buffer, address1, 4));
    status = DHCPAllocIP(dhcp, address1, buffer);
    TEST1(status, SUCCESS_OTHER_IP);
    TEST1(0, memcmp(buffer, address2, 4));
    status = DHCPAllocIP(dhcp, address2, buffer);
    TEST1(status, SUCCESS_OTHER_IP);
    TEST1(0, memcmp(buffer, address3, 4));
    status = DHCPAllocIP(dhcp, address3, buffer);
    TEST1(status, SUCCESS_OTHER_IP);
    TEST1(0, memcmp(buffer, address4, 4));
    status = DHCPAllocIP(dhcp, address4, buffer);
    TEST1(status, SUCCESS_OTHER_IP);
    TEST1(0, memcmp(buffer, address5, 4));
    status = DHCPAllocIP(dhcp, address5, buffer);
    TEST1(status, DHCP_FULL);
    status = DHCPAllocIP(dhcp, address6, buffer);
    TEST1(status, DHCP_FULL);
    status = DHCPAllocIP(dhcp, address7, buffer);
    TEST1(status, DHCP_FULL);
    DHCPDestroy(dhcp);
}
     

