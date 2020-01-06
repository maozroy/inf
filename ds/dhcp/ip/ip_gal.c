/****************************************************************************
                                                                           
			                 IP	                        
                             OL79 - 5/1/20                              
                                                                           
                             Writer:    Gal Salemon                       
                             Reviewer:  Maoz                   
*****************************************************************************/
#include <assert.h>

#include "../trie/trie.h" 
#include "ip.h"

#define BYTE_SIZE (8)
#define BITS_IN_INT (32)

BinaryIp IPtoBit(IPAddress address)
{
	int i = 0;
	BinaryIp result = 0;
	
	for (i = 0; i < IP_SIZE; ++i)
	{
		result <<= BYTE_SIZE;
		result |= address[i];
	}
	
	return result;
}

void BitToIp(BinaryIp ip, IPAddress result_ip)
{
	int i = 0;
	size_t shift = BYTE_SIZE * 3;
	
	for (i = 0; i < IP_SIZE; ++i)
	{
		result_ip[i] = ip >> shift;
		shift -= BYTE_SIZE;
	}
}

void CutSubnet(IPAddress address, size_t mask)
{
	BinaryIp binary_address = IPtoBit(address);
	
	mask = BITS_IN_INT - mask;
	binary_address >>= mask;
	binary_address <<= mask;
	
	BitToIp(binary_address, address);
}

void AddSubnet(IPAddress subnet, size_t mask, BinaryIp added_part, IPAddress result)
{
	BinaryIp binary_subnet = 0;
	
	CutSubnet(subnet, mask);
	binary_subnet = IPtoBit(subnet);
	
	added_part <<= mask;
	added_part >>= mask;
	
	binary_subnet ^= added_part;
	
	BitToIp(binary_subnet, result);
}

int IsValid(IPAddress subnet, IPAddress address, size_t mask)
{
	BinaryIp binary_address = IPtoBit(address);
	BinaryIp binary_subnet = IPtoBit(subnet);
	
	assert((mask > 0) && (mask < BITS_IN_INT));
	
	mask = BITS_IN_INT - mask;
	binary_address >>= mask;
	binary_address <<= mask;
	binary_subnet >>= mask;
	binary_subnet <<= mask;
	
	return (binary_subnet == binary_address);
}



