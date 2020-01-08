/**********************************************************************
 *	This code is representing my tests of IP        	              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 05.01.20				        							  *
 **********************************************************************/
#include <stdio.h>	    /*printf*/
#include <string.h>	    /*memcmp*/

#include "ip.h"

#include "../../../yoni_utils.h" /*RUN_SEQUENCE, RUN_TEST*/

#define BYTE (8)

int IPtoBitTest();
int BitToIpTest();
int CutSubnetTest();
int IsValidTest();

int main()
{
	RUN_SEQUENCE("IPtoBitTest", IPtoBitTest());
	RUN_SEQUENCE("BitToIpTest", BitToIpTest());
	RUN_SEQUENCE("CutSubnetTest", CutSubnetTest());
	RUN_SEQUENCE("IsValidTest", IsValidTest());

	return 0;
}

int IPtoBitTest()
{
    /*ip_address[0] = 0000 0011 -- 3*/
    /*ip_address[1] = 0010 0110 -- 38*/
    /*ip_address[2] = 0000 1111 -- 15*/
    /*ip_address[3] = 1110 1110 -- 238*/
    
    /*binary_address = ip_address[0], ip_address[1], ip_address[2], ip_address[3]*/
    /*binary_address =             3,            38,            15,          238*/
    /*binary_address = 0000 0011 ,0010 0110 ,0000 1111 ,1110 1110*/
    BinaryIp binary_address = 0;
    
    IPAddress ip_address = {0};
    ip_address[0] = 3;
    ip_address[1] = 38;
    ip_address[2] = 15;
    ip_address[3] = 238;

        
    binary_address = IPtoBit(ip_address);
        
	RUN_TEST("Test1", 52826094 == binary_address);

    return 0;
}

int BitToIpTest()
{
    /*ip_address[0] = 0000 0011 -- 3*/
    /*ip_address[1] = 0010 0110 -- 38*/
    /*ip_address[2] = 0000 1111 -- 15*/
    /*ip_address[3] = 1110 1110 -- 238*/
    
    /*binary_address = ip_address[0], ip_address[1], ip_address[2], ip_address[3]*/
    /*binary_address =             3,            38,            15,          238*/
    /*binary_address = 0000 0011 ,0010 0110 ,0000 1111 ,1110 1110*/
    BinaryIp binary_address = 52826094;
    IPAddress result_ip_address = {0};
    IPAddress ip_address_to_compare = {0};
    ip_address_to_compare[0] = 3;
    ip_address_to_compare[1] = 38;
    ip_address_to_compare[2] = 15;
    ip_address_to_compare[3] = 238;

    BitToIp(binary_address, result_ip_address);
        
	RUN_TEST("Test1", 0 == memcmp(ip_address_to_compare, result_ip_address, 32/BYTE));

    return 0;
}

int CutSubnetTest()
{
    /*ip_address[0] = 0000 0011 -- 3*/
    /*ip_address[1] = 0010 0110 -- 38*/
    /*ip_address[2] = 0000 1111 -- 15*/
    /*ip_address[3] = 1110 1110 -- 238*/
    
    IPAddress ip_address1_src = {3, 38, 15, 238};
    IPAddress ip_address2_src = {3, 38, 15, 238};
    IPAddress ip_address3_src = {3, 38, 15, 238};
    IPAddress ip_address4_src = {3, 38, 15, 238};
    IPAddress ip_address5_src = {3, 38, 15, 238};
    
    IPAddress ip_address1_res = {3, 38, 15, 238}; /*cut 1 bit, 31 mask*/
    IPAddress ip_address2_res = {3, 38, 15, 0};   /*cut 8 bit, 24 mask*/
    IPAddress ip_address3_res = {3, 38, 14, 0};   /*cut 9 bit, 23 mask*/
    IPAddress ip_address4_res = {3, 38, 0, 0};    /*cut 16 bit, 16 mask*/
    IPAddress ip_address5_res = {3, 32, 0, 0};    /*cut 20 bit, 12 mask*/

    CutSubnet(ip_address1_src, 31);
    CutSubnet(ip_address2_src, 24);
    CutSubnet(ip_address3_src, 23);
    CutSubnet(ip_address4_src, 16);
    CutSubnet(ip_address5_src, 12);
        
	RUN_TEST("Test1", 0 == memcmp(ip_address1_src, ip_address1_res, 32/BYTE));
	RUN_TEST("Test2", 0 == memcmp(ip_address2_src, ip_address2_res, 32/BYTE));
    RUN_TEST("Test3", 0 == memcmp(ip_address3_src, ip_address3_res, 32/BYTE));
	RUN_TEST("Test4", 0 == memcmp(ip_address4_src, ip_address4_res, 32/BYTE));
	RUN_TEST("Test5", 0 == memcmp(ip_address5_src, ip_address5_res, 32/BYTE));
	
    return 0;
}

int IsValidTest()
{
    IPAddress ip_address1 = {3, 38, 15, 238};
    IPAddress ip_address2 = {3, 38, 15, 238};
    IPAddress ip_address3 = {3, 38, 15, 238};
    IPAddress ip_address4 = {3, 38, 15, 238};
    IPAddress ip_address5 = {3, 38, 15, 238};
    
    IPAddress ip_address1_subnet = {3, 38, 15, 239};
    IPAddress ip_address2_subnet = {3, 38, 15, 0};
    IPAddress ip_address3_subnet = {3, 38, 14, 0};
    IPAddress ip_address4_subnet = {3, 38, 0, 0};
    IPAddress ip_address5_subnet = {3, 35, 0, 0};

	RUN_TEST("Test1", 1 == IsValid(ip_address1, ip_address1_subnet, 31));
	RUN_TEST("Test2", 0 == IsValid(ip_address2, ip_address2_subnet, 32));
    RUN_TEST("Test3", 1 == IsValid(ip_address3, ip_address3_subnet, 23));
	RUN_TEST("Test4", 1 == IsValid(ip_address4, ip_address4_subnet, 16));
	RUN_TEST("Test5", 1 == IsValid(ip_address5, ip_address5_subnet, 13));
	
    return 0;
}
