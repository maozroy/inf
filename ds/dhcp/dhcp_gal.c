/****************************************************************************
                                                                           
			                 DHCP	                        
                             OL79 - 5/1/20                              
                                                                           
                             Writer:    Gal Salemon                       
                             Reviewer:  Maoz                   
*****************************************************************************/
#include <stdlib.h> /* size_t */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "dhcp.h"
#include "./trie/trie.h" 
#include "./ip/ip.h"

#define BITS_IN_INT (32)
#define SUCCESS (0)
#define FAIL (1)
#define NOT_VALID (0)
#define VALID (1)

struct dhcp
{
	trie_t *trie;
	IPAddress subnet;
	size_t mask;	
};

static int InitTrieIMP(trie_t *trie);
static int InitDhcpIMP(dhcp_t *dhcp, const IPAddress subnet, size_t mask);
static alloc_status_t AllocDifferentIPIMP(dhcp_t *dhcp, 
										  IPAddress result_address);

dhcp_t *DHCPCreate(const IPAddress subnet, size_t mask)
{
	int init_status = SUCCESS;
	dhcp_t *dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
	if (NULL == dhcp)
	{
		return NULL;
	}

	init_status = InitDhcpIMP(dhcp, subnet, mask);
	if (FAIL == init_status)
	{
		TrieDestroy(dhcp->trie);
		free(dhcp);
		
		return NULL;
	}

	return dhcp;
}

static int InitDhcpIMP(dhcp_t *dhcp, const IPAddress subnet, size_t mask)
{	
	int status = SUCCESS;	
	
	dhcp->trie = TrieCreate(BITS_IN_INT - mask);
	if (NULL == dhcp)
	{
		return FAIL;
	}
	
	dhcp->mask = mask;
	memcpy(dhcp->subnet, subnet, IP_SIZE);	
	status = InitTrieIMP(dhcp->trie);
	
	return status;
}

static int InitTrieIMP(trie_t *trie)
{
	int status = SUCCESS;
	
	status = TrieInsert(trie, 0);
	if (status != T_SUCCESS_ALLOCATED_REQUESTED)
	{
		status = FAIL;
	}
	
	status = TrieInsert(trie, ~0);
	if (status != T_SUCCESS_ALLOCATED_REQUESTED)
	{
		status = FAIL;
	}
	
	status = TrieInsert(trie, ~0 - 1);
	if (status != T_SUCCESS_ALLOCATED_REQUESTED)
	{
		status = FAIL;
	}
	
	return status;
}

void DHCPDestroy(dhcp_t *dhcp)
{
	assert(dhcp);
	
	TrieDestroy(dhcp->trie);
	free(dhcp);
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
	assert(dhcp);
	
	return TrieCountFree(dhcp->trie);
}

alloc_status_t DHCPAllocIP(dhcp_t *dhcp, const IPAddress requested, 
						   IPAddress result_address)
{	
	int valid_status = VALID;
	trie_alloc_status_t trie_status = T_SUCCESS_ALLOCATED_REQUESTED;	
	BinaryIp binary_requested = IPtoBit(*(IPAddress*)requested);
		
	assert(dhcp);
	assert(result_address);
	
	if (0 == TrieCountFree(dhcp->trie))
	{
		result_address = NULL;
		
		return DHCP_FULL;
	}
	
	if (0 == binary_requested)
	{
		return AllocDifferentIPIMP(dhcp, result_address);
	}
	
	valid_status = IsValid(dhcp->subnet, *(IPAddress*)requested, dhcp->mask);
	if (NOT_VALID == valid_status)
	{
		result_address = NULL;
		
		return INVALID_IP;
	}
	
	trie_status = TrieInsert(dhcp->trie, binary_requested);
	if (T_SUCCESS_ALLOCATED_REQUESTED == trie_status)
	{
		BitToIp(binary_requested, result_address);
		
		return SUCCESS_ALLOCATED_REQUESTED;
	}
	else if (T_MALLOC_FAIL == trie_status)
	{
		result_address = NULL;
		
		return MALLOC_FAIL;
	}
	
	return AllocDifferentIPIMP(dhcp, result_address);
}

static alloc_status_t AllocDifferentIPIMP(dhcp_t *dhcp, 
										  IPAddress result_address)
{
	int valid_status = VALID;
	trie_alloc_status_t trie_status = T_SUCCESS_ALLOCATED_REQUESTED;
	BinaryIp binary_subnet = 0;
	IPAddress valid_ip = {0};
	
	AddSubnet(dhcp->subnet, dhcp->mask, 0, valid_ip);
	binary_subnet = IPtoBit(valid_ip);
		
	while (VALID == valid_status)
	{
		binary_subnet += 1;
		trie_status = TrieInsert(dhcp->trie, binary_subnet);
		
		if (T_SUCCESS_ALLOCATED_REQUESTED == trie_status)
		{
			BitToIp(binary_subnet, result_address);
	
			return SUCCESS_OTHER_IP;
		}			
		if (T_MALLOC_FAIL == trie_status)
		{
			result_address = NULL;
			
			return MALLOC_FAIL;
		}			
		
		BitToIp(binary_subnet, valid_ip);
		valid_status = IsValid(dhcp->subnet, valid_ip, dhcp->mask);
	}		
	
	result_address = NULL;
			
	return INVALID_IP;
}

free_status_t DHCPFreeIP(dhcp_t *dhcp, const IPAddress address_to_free)
{
	int valid_status = VALID;
	trie_free_status_t trie_status = T_SUCCESS;
	BinaryIp binary_address_to_free = IPtoBit(*(IPAddress*)address_to_free);
	
	valid_status = IsValid(dhcp->subnet, 
						 *(IPAddress*)address_to_free, 
						 dhcp->mask);
	if (NOT_VALID == valid_status)
	{
		return INVALID_SUBNET;
	}
	
	trie_status = TrieDeallocate(dhcp->trie, binary_address_to_free);
	if (T_DOUBLE_FREE == trie_status)
	{
		return DOUBLE_FREE;
	}
	if (T_IP_NOT_FOUND == trie_status)
	{
		return IP_NOT_FOUND;
	}
	
	return SUCCESS;
}


