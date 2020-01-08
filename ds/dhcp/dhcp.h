/****************************************************************************
*                                                                           *
*                               OL79 - 02/01/19                             *
*                                                                           *
*                                 DHCP										*
*                                                                           *
*****************************************************************************/

#ifndef ILRD_DHCP_H
#define ILRD_DHCP_H

#include <stddef.h> /* size_t */

#include "trie/trie.h" 
#include "ip/ip.h"

typedef struct dhcp dhcp_t;

typedef enum dhcp_status_free
{
	SUCCESS = 0,
	DOUBLE_FREE = 1,
	IP_NOT_FOUND = 2,
	INVALID_SUBNET = 3
}free_status_t;

typedef enum dhcp_status_alloc
{
	SUCCESS_ALLOCATED_REQUESTED = 0,
	SUCCESS_OTHER_IP = 1,
	INVALID_IP = 2,
	DHCP_FULL = 3,
	MALLOC_FAIL = 4
}alloc_status_t;

/* * * * * * MOVE TO SOURCE FILE !! * * * * 
struct dhcp
{
	trie_t *trie;
	IPAddress subnet;
	size_t mask;	
};
* * * * * * * * * * * * * * * * * * * * */


/* Function creates a dhcp.
 * Receives a subnet and a mask.
 * mask must be lower than 32. 
 * Return Values:
 *               - returns pointer to the dhcp.
 *               - if the function fails, it returns NULL. */
dhcp_t *DHCPCreate(const IPAddress subnet, size_t mask);

/* The function destroys the dhcp and frees all memory allocations. */
void DHCPDestroy(dhcp_t *dhcp);

/*	The function recieves ip address requested and ip address result.
*	Function allocates ip address.
*	The allocated address will be saved in the result address  
*	If requested address equal 0, the result address will not be affected by it.
*	If allocation fails result address will be NULL */
alloc_status_t DHCPAllocIP(dhcp_t *dhcp, const IPAddress requested, IPAddress result_address);

/*	Frees address to free.
*	If address does not exist return value will be IP_NOT_FOUND */
free_status_t DHCPFreeIP(dhcp_t *dhcp, const IPAddress address_to_free);

/* Counts number of available addresses in the subnet */
size_t DHCPCountFree(const dhcp_t *dhcp);


#endif
