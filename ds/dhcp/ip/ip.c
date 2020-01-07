 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : 	Sharon Rottner
*	ip					
*	Date: 6/1/2020              
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <string.h> /*memcmp*/

#include "ip.h"

#define MASK_1_BYTE 255
#define BITS_IN_BYTE (8)
#define BITS_IN_ADRS ((BITS_IN_BYTE) * (IP_SIZE))

BinaryIp IPtoBit(IPAddress address)
{
	size_t i = 0;
	BinaryIp bit = 0;
	
	assert(address);
	
	for (i = 0; i < IP_SIZE ; ++i)
	{
		bit += address[i]<<(BITS_IN_BYTE * (IP_SIZE - i - 1));
	}
	return bit;
}

void BitToIp(BinaryIp ip, IPAddress result_ip)
{
	size_t i = 0;
	unsigned char temp = 0;
	size_t num_of_shift = 0;
	
	assert(result_ip);
	
	for (i = 0; i < IP_SIZE ; ++i)
	{
		num_of_shift = (BITS_IN_BYTE * (IP_SIZE - i - 1));
		temp = ((MASK_1_BYTE<<num_of_shift)&ip)>>num_of_shift;
		result_ip[i] = temp;
	}
}

void CutSubnet(IPAddress address, size_t mask)
{
	BinaryIp bit = 0;
	
	assert(address);
	
	bit = IPtoBit(address);
	bit = bit << mask;
	bit = bit >> mask;
	
	BitToIp(bit, address);
	
 	return;
}

int IsValid(IPAddress subnet, IPAddress address, size_t mask)
{
	BinaryIp bit_adrs = 0;
	BinaryIp bit_subnet = 0;
	
	assert(subnet);
	assert(address);
	
	bit_adrs = IPtoBit(address);
	bit_subnet = IPtoBit(subnet);
	bit_adrs = bit_adrs >> (BITS_IN_ADRS - mask);
	bit_adrs = bit_adrs << (BITS_IN_ADRS - mask);
	bit_subnet = bit_subnet >> (BITS_IN_ADRS - mask);
	bit_subnet = bit_subnet << (BITS_IN_ADRS - mask);
	
	return (bit_subnet == bit_adrs);
}

void AddSubnet(IPAddress subnet, size_t mask, unsigned int added_part, IPAddress result)
{
	BinaryIp subnet_ip = 0;
	
	assert(subnet);
	assert(result);
	
	subnet_ip = IPtoBit(subnet);
	added_part <<= mask;
	added_part >>= mask;
	subnet_ip >>= (BITS_IN_ADRS) - mask;
	subnet_ip <<= (BITS_IN_ADRS) - mask;
	subnet_ip |= added_part;
	BitToIp(subnet_ip, result);
}

