/**********************************************************************
 *	This code is representing my tests of DHCP      	              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 06.01.20						        					  *
 **********************************************************************/
#include <stdio.h>	    /*printf*/
#include <assert.h>	    /*assert*/
#include <string.h>	    /*memcmp*/

#include "dhcp.h"

#include "../../yoni_utils.h" /*RUN_SEQUENCE, RUN_TEST*/

int DHCPCreateDestroyTest();
int DHCPAllocIPTest30mask();
int DHCPAllocIPTest29mask();
int DHCPFreeIPTest();
int DHCPCountFreeTest();


int main()
{
	RUN_SEQUENCE("DHCPCreateDestroyTest", DHCPCreateDestroyTest());
	RUN_SEQUENCE("DHCPAllocIPTest30mask", DHCPAllocIPTest30mask());
	RUN_SEQUENCE("DHCPAllocIPTest29mask", DHCPAllocIPTest29mask());
	RUN_SEQUENCE("DHCPFreeIPTest", DHCPFreeIPTest());
	RUN_SEQUENCE("DHCPCountFreeTest", DHCPCountFreeTest());

	return 0;
}

int DHCPCreateDestroyTest()
{
    IPAddress subnet = {0,0,0,0};
    dhcp_t *dhcp = DHCPCreate(subnet, 30);
    assert(dhcp);
    
    printf("---Check Valgrind---\n");
    
    DHCPDestroy(dhcp);
	
	return 0;
}

int DHCPAllocIPTest30mask()
{
    alloc_status_t return_status = 0;
    IPAddress subnet = {0,0,0,0};
    IPAddress req_address = {0,0,0,0};
    IPAddress invalid_req_address = {0,0,0,4};
    IPAddress res_address = {0,0,0,0};
    dhcp_t *dhcp = DHCPCreate(subnet, 30);
    assert(dhcp);
    
    return_status = DHCPAllocIP(dhcp, invalid_req_address, res_address);
	RUN_TEST("Invalid request", INVALID_IP == return_status);
    
    /*only second leaf is free to use, others are taken by default*/
    return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc 1", SUCCESS_OTHER_IP == return_status);
	req_address[3] += 1;
	RUN_TEST("Alloc 1 res", 0 == memcmp(req_address ,res_address, 4));
	
	return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc when full", DHCP_FULL == return_status);
    
    DHCPDestroy(dhcp);

	return 0;
}

int DHCPAllocIPTest29mask()
{
    alloc_status_t return_status = 0;
    IPAddress subnet = {0,0,0,0};
    IPAddress req_address = {0,0,0,1};
    IPAddress invalid_req_address = {0,0,0,8};
    IPAddress res_address = {0,0,0,0};
    dhcp_t *dhcp = DHCPCreate(subnet, 29);
    assert(dhcp);
    
    return_status = DHCPAllocIP(dhcp, invalid_req_address, res_address);
	RUN_TEST("Invalid request", INVALID_IP == return_status);
    
    return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc 1", SUCCESS_ALLOCATED_REQUESTED == return_status);
	RUN_TEST("Alloc 1 res", 0 == memcmp(req_address ,res_address, 4));
	printf("return_status: %d\n", return_status);
	
	return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc 2", SUCCESS_OTHER_IP == return_status);
	req_address[3] += 1;
	RUN_TEST("Alloc 2 res", 0 == memcmp(req_address ,res_address, 4));
	printf("return_status: %d\n", return_status);

	return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc 3", SUCCESS_OTHER_IP == return_status);
	req_address[3] += 1;
	RUN_TEST("Alloc 3 res", 0 == memcmp(req_address ,res_address, 4));
	printf("return_status: %d\n", return_status);
	
	return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc 4", SUCCESS_OTHER_IP == return_status);
	req_address[3] += 1;
	RUN_TEST("Alloc 4 res", 0 == memcmp(req_address ,res_address, 4));
	printf("return_status: %d\n", return_status);
	
	return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc 5", SUCCESS_OTHER_IP == return_status);
	req_address[3] += 1;
	RUN_TEST("Alloc 5 res", 0 == memcmp(req_address ,res_address, 4));
	printf("return_status: %d\n", return_status);
	
	return_status = DHCPAllocIP(dhcp, req_address, res_address);
	RUN_TEST("Alloc when full", DHCP_FULL == return_status);
    
    DHCPDestroy(dhcp);

	return 0;
}

int DHCPFreeIPTest()
{
    free_status_t return_status = 0;
    IPAddress subnet = {0,0,0,0};
    IPAddress req_address = {0,0,0,1};
    IPAddress invalid_req_address = {0,0,0,8};
    IPAddress res_address1 = {0,0,0,0};
    IPAddress res_address2 = {0,0,0,0};
    IPAddress res_address3 = {0,0,0,0};
    IPAddress res_address4 = {0,0,0,0};
    IPAddress res_address5 = {0,0,0,0};
    
    dhcp_t *dhcp = DHCPCreate(subnet, 29);
    assert(dhcp);
    
    return_status = DHCPFreeIP(dhcp, invalid_req_address);
	RUN_TEST("Free Invalid", INVALID_SUBNET == return_status);
    
    return_status = DHCPFreeIP(dhcp, req_address);
	RUN_TEST("Free Not Alloced", IP_NOT_FOUND == return_status);
    
    DHCPAllocIP(dhcp, req_address, res_address1);
    DHCPAllocIP(dhcp, req_address, res_address2);
    DHCPAllocIP(dhcp, req_address, res_address3);
    DHCPAllocIP(dhcp, req_address, res_address4);
    DHCPAllocIP(dhcp, req_address, res_address5);
	
    return_status = DHCPFreeIP(dhcp, req_address);
	RUN_TEST("Free 1", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address1);
	RUN_TEST("Double Free 1", DOUBLE_FREE == return_status);
	
	return_status = DHCPFreeIP(dhcp, res_address2);
	RUN_TEST("Free 2", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address2);
	RUN_TEST("Double Free 2", DOUBLE_FREE == return_status);
	
	return_status = DHCPFreeIP(dhcp, res_address3);
	RUN_TEST("Free 3", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address3);
	RUN_TEST("Double Free 3", DOUBLE_FREE == return_status);
	
	return_status = DHCPFreeIP(dhcp, res_address4);
	RUN_TEST("Free 4", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address4);
	RUN_TEST("Double Free 4", DOUBLE_FREE == return_status);
	
	return_status = DHCPFreeIP(dhcp, res_address5);
	RUN_TEST("Free 5", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address5);
	RUN_TEST("Double Free 5", DOUBLE_FREE == return_status);
	
	printf("\n---Allocations after freeing---\n\n");
	
	DHCPAllocIP(dhcp, req_address, res_address1);
    DHCPAllocIP(dhcp, req_address, res_address2);
    DHCPAllocIP(dhcp, req_address, res_address3);
	
	return_status = DHCPFreeIP(dhcp, req_address);
	RUN_TEST("Free 1", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address1);
	RUN_TEST("Double Free 1", DOUBLE_FREE == return_status);
	
	return_status = DHCPFreeIP(dhcp, res_address2);
	RUN_TEST("Free 2", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address2);
	RUN_TEST("Double Free 2", DOUBLE_FREE == return_status);
	
	return_status = DHCPFreeIP(dhcp, res_address3);
	RUN_TEST("Free 3", SUCCESS == return_status);
    return_status = DHCPFreeIP(dhcp, res_address3);
	RUN_TEST("Double Free 3", DOUBLE_FREE == return_status);
	
    DHCPDestroy(dhcp);

	return 0;
}

int DHCPCountFreeTest()
{
    IPAddress subnet = {0,0,0,0};
    IPAddress req_address = {0,0,0,1};
    IPAddress res_address1 = {0,0,0,0};
    IPAddress res_address2 = {0,0,0,0};
    IPAddress res_address3 = {0,0,0,0};
    IPAddress res_address4 = {0,0,0,0};
    IPAddress res_address5 = {0,0,0,0};
     
    dhcp_t *dhcp = DHCPCreate(subnet, 29);
    assert(dhcp);
    
    RUN_TEST("Before Allocs", 5 == DHCPCountFree(dhcp));
    DHCPAllocIP(dhcp, req_address, res_address1);
    RUN_TEST("Alloc 1", 4 == DHCPCountFree(dhcp));
    DHCPAllocIP(dhcp, req_address, res_address2);
    RUN_TEST("Alloc 2", 3 == DHCPCountFree(dhcp));
    DHCPAllocIP(dhcp, req_address, res_address3);
    RUN_TEST("Alloc 3", 2 == DHCPCountFree(dhcp));
    DHCPAllocIP(dhcp, req_address, res_address4);
    RUN_TEST("Alloc 4", 1 == DHCPCountFree(dhcp));
    DHCPAllocIP(dhcp, req_address, res_address5);
    RUN_TEST("Alloc 5", 0 == DHCPCountFree(dhcp));	
	
    DHCPFreeIP(dhcp, res_address1);
    RUN_TEST("Free 1", 1 == DHCPCountFree(dhcp));
    DHCPFreeIP(dhcp, res_address2);
    RUN_TEST("Free 2", 2 == DHCPCountFree(dhcp));
    DHCPFreeIP(dhcp, res_address3);
    RUN_TEST("Free 3", 3 == DHCPCountFree(dhcp));
    DHCPFreeIP(dhcp, res_address4);
    RUN_TEST("Free 4", 4 == DHCPCountFree(dhcp));
    DHCPFreeIP(dhcp, res_address5);
    RUN_TEST("Free 5", 5 == DHCPCountFree(dhcp));
    
	
	DHCPDestroy(dhcp);

	return 0;
}
