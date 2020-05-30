 /********************************************
*	Author : Maoz roytman		
*	Reviewer : 	Sharon Rottner
*	dhcp					
*	Date: 6/1/2020		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <string.h> /*memcpy*/

#include "dhcp.h"
#include "trie/trie.h" /*insert, create, free, destroy*/
#include "ip/ip.h" /*IPAddresses*/

#define BITS_IN_BYTE 8
#define BITS_IN_IP (BITS_IN_BYTE * IP_SIZE)
#define DEFAULT_ADRS {0, 0, 0, 0}

struct dhcp
{
	trie_t *trie;
	IPAddress subnet;
	size_t mask;	
};

dhcp_t *DHCPCreate(const IPAddress subnet, size_t mask)
{
	dhcp_t *new_dhcp = NULL;
	
	assert(mask < BITS_IN_IP);
	
	new_dhcp = malloc(sizeof(dhcp_t));
	if (NULL == new_dhcp)
	{
		return NULL;
	}
	
	new_dhcp -> trie = TrieCreate(BITS_IN_IP - mask);
	if (NULL == new_dhcp -> trie)
	{
		free(new_dhcp);
		return NULL;
	}
	
	memcpy(new_dhcp -> subnet, subnet, IP_SIZE);
	new_dhcp -> mask = mask;
	
	if ((T_SUCCESS_ALLOCATED_REQUESTED != TrieInsert(new_dhcp -> trie, 0)) ||
		(T_SUCCESS_ALLOCATED_REQUESTED != TrieInsert(new_dhcp -> trie, ~0)) ||
		(T_SUCCESS_ALLOCATED_REQUESTED != TrieInsert(new_dhcp -> trie, ~0 - 1)))
	{
		DHCPDestroy(new_dhcp);
		return NULL;
	}
		
	return new_dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
	assert(dhcp);
	
	TrieDestroy(dhcp -> trie);
	free(dhcp);
}

alloc_status_t DHCPAllocIP(dhcp_t *dhcp, const IPAddress requested,
						   IPAddress result_address)
{
	trie_alloc_status_t alloc_status = 0;
	alloc_status_t dhcp_status = 0;
	BinaryIp bit_adrs = 0;
	IPAddress temp_adrs = {0};
	IPAddress default_adrs = DEFAULT_ADRS;
	
	assert(dhcp);
	assert(result_address);
	assert(requested);
	
	if (TrieIsFull(dhcp -> trie))
	{
		result_address = NULL;
		return DHCP_FULL;
	}
	
	if (IsValid(default_adrs, (unsigned char *)requested, BITS_IN_IP))
	{
		bit_adrs = IPtoBit(default_adrs);
	}
	else if (IsValid(dhcp -> subnet, (unsigned char *)requested, dhcp -> mask))
	{
		memcpy(temp_adrs, requested, IP_SIZE);
		bit_adrs = IPtoBit(temp_adrs);
	}
	else
	{
		result_address = NULL;
		return INVALID_IP;
	}
	
	alloc_status = TrieInsert(dhcp -> trie, bit_adrs);
	
	if (T_REQUESTED_IP_OCCUPIED == alloc_status)
	{
		bit_adrs = 0;
		
		while (T_REQUESTED_IP_OCCUPIED == alloc_status)
		{
			++bit_adrs;
			alloc_status = TrieInsert(dhcp -> trie, bit_adrs);
			
			if (T_MALLOC_FAIL == alloc_status)
			{
				return T_MALLOC_FAIL;
			}
		}
		dhcp_status = SUCCESS_OTHER_IP;
	}
	AddSubnet(dhcp -> subnet, dhcp -> mask, bit_adrs, result_address);
	
	if (T_MALLOC_FAIL == alloc_status)
	{
		dhcp_status = MALLOC_FAIL;
	}
	
	return dhcp_status;
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
	assert(dhcp);

	return TrieCountFree(dhcp -> trie);
}

free_status_t DHCPFreeIP(dhcp_t *dhcp, const IPAddress address_to_free)
{
	trie_free_status_t trie_status = {0};
	
	assert(dhcp);
	assert(address_to_free);
	
	if (!IsValid(dhcp -> subnet, 
				(unsigned char *)address_to_free, dhcp -> mask))
	{
		return INVALID_SUBNET;
	}
	
	trie_status = TrieDeallocate(dhcp -> trie, 
								 IPtoBit((unsigned char *)address_to_free));
	
	switch (trie_status)
	{
		case T_DOUBLE_FREE:
			return DOUBLE_FREE;		
			
		case T_IP_NOT_FOUND:
			return IP_NOT_FOUND;
	}
	return SUCCESS;
}

