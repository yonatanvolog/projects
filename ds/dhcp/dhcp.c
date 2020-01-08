/**********************************************************************
 *	This code is representing my functions of DHCP RC2                *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Erez Barr             	 	  	    		     	  *
 *	Date: 06.01.20  												  *
 **********************************************************************/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <string.h> /*memcpy*/
#include <math.h>   /*pow*/

#include "dhcp.h"

#define BYTE_SIZE (8)
#define BITS_IN_IP ((IP_SIZE)*(BYTE_SIZE))

struct dhcp
{
	trie_t *trie;
	IPAddress subnet;
	size_t mask;	
};

static alloc_status_t AllocReservedAddressesIMP(trie_t *trie);

dhcp_t *DHCPCreate(const IPAddress subnet, size_t mask)
{
    dhcp_t *new_dhcp = NULL;
    
    assert(subnet);
    assert(BITS_IN_IP > mask);
    
    new_dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
    if (NULL == new_dhcp)
    {
        return NULL;
    }
    new_dhcp->trie = TrieCreate(BITS_IN_IP - mask);
    
    if (NULL == new_dhcp->trie)
    {
        DHCPDestroy(new_dhcp);
        
        return NULL;
    }

    memcpy(new_dhcp->subnet, subnet, IP_SIZE);
	new_dhcp->mask = mask;
    CutSubnet(new_dhcp->subnet, new_dhcp->mask);
    	
	if (SUCCESS_ALLOCATED_REQUESTED != AllocReservedAddressesIMP(new_dhcp->trie))
    {
        free(new_dhcp);
        
        return NULL; 
    }
    
    return new_dhcp;
}

static alloc_status_t AllocReservedAddressesIMP(trie_t *trie)
{
    int alloc_status = 0;
    
    IPAddress first_ip = {0,0,0,0};
    IPAddress pre_last_ip = {255,255,255,254};
    IPAddress last_ip = {255,255,255,255};
    
    BinaryIp first_ip_in_bit = IPtoBit(first_ip);
    BinaryIp pre_last_ip_bit = IPtoBit(pre_last_ip);
    BinaryIp last_ip_bit = IPtoBit(last_ip);
    
    /*TrieInsert is used intead of DHCPAllocIP 
      in order to insert non-valid starting addresses.*/
    alloc_status += TrieInsert(trie, first_ip_in_bit);
    alloc_status += TrieInsert(trie, pre_last_ip_bit);
    alloc_status += TrieInsert(trie, last_ip_bit);

    return alloc_status;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    TrieDestroy(dhcp->trie);
    free(dhcp);
}

alloc_status_t DHCPAllocIP(dhcp_t *dhcp, const IPAddress requested, 
                                          IPAddress result_address)
{
    unsigned int i = 0;
    size_t num_of_free_nodes = pow(2,(BITS_IN_IP - dhcp->mask));
    BinaryIp requested_in_bit = 0;
    trie_alloc_status_t trie_return_status = MALLOC_FAIL;
    
    assert(dhcp);
    assert(requested);
    assert(result_address);

    if (!IsValid(dhcp->subnet, requested, dhcp->mask))
    {
        return INVALID_IP;
    }
    
    requested_in_bit = IPtoBit(requested);
    
    for (i = 0; i <= num_of_free_nodes; ++i)
    {
        trie_return_status = TrieInsert(dhcp->trie, requested_in_bit + i);
        switch (trie_return_status)
        {
            case T_MALLOC_FAIL:
                return MALLOC_FAIL;
                break;
                
            case T_SUCCESS_ALLOCATED_REQUESTED:
                BitToIp((requested_in_bit + i), result_address);
                if (0 == i)
                {
                    return SUCCESS_ALLOCATED_REQUESTED;
                }
                return SUCCESS_OTHER_IP;
                break;
                
            case T_REQUESTED_IP_OCCUPIED:            
                requested_in_bit = IPtoBit(dhcp->subnet);
                break;
        }
    }
    
    return DHCP_FULL;
}

free_status_t DHCPFreeIP(dhcp_t *dhcp, const IPAddress address_to_free)
{
    BinaryIp address_to_free_in_bit = 0;
    
    assert(dhcp);
    
    if (!IsValid(dhcp->subnet, address_to_free, dhcp->mask))
    {
        return INVALID_SUBNET;
    }
    
    address_to_free_in_bit = IPtoBit(address_to_free);
    
    return TrieDeallocate(dhcp->trie, address_to_free_in_bit);
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(dhcp);
    
    return TrieCountFree(dhcp->trie);
}
