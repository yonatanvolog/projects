/**********************************************************************
 *	This code is representing my functions of IP RC2                  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Erez Barr             	 	  	    		     	  *
 *	Date: 05.01.20  												  *
 **********************************************************************/
#include "ip.h"

#define BYTE_SIZE (8)
#define BITS_IN_IP ((IP_SIZE)*(BYTE_SIZE))
#define TURNED_ON_BYTE (15)

BinaryIp IPtoBit(const IPAddress address)
{
    size_t i = 0;
    BinaryIp converted = 0;
    BinaryIp mask = 0xff;       /*turns on all bits in the LSB*/

    for (i = 0; i < IP_SIZE; ++i)
    {
        converted <<= BYTE_SIZE;
        converted ^= (address[i] & mask);
    }
    
    return converted;
}

void BitToIp(BinaryIp ip, IPAddress result_ip)
{
    size_t i = 0;
    BinaryIp mask = 0xff; /*2 most significant bytes are on*/
    
    for(i = 0; i < IP_SIZE; ++i)
    {
        result_ip[i] = (ip >> (IP_SIZE - 1 - i) * BYTE_SIZE) & mask;
    }
}

void CutSubnet(IPAddress address, size_t mask)
{
    size_t i = 0, j = IP_SIZE - 1;
    char bit_mask[8] =  {0,         /*filler*/
                        (char)254,  /*1111 1110 - turn off one bit at end*/
                        (char)252,  /*1111 1100*/
                        (char)248,  /*1111 1000*/             
                        (char)240,  /*1111 0000*/  
                        (char)224,  /*1110 0000*/
                        (char)192,  /*1100 0000*/
                        (char)128}; /*1000 0000*/

    mask = BITS_IN_IP - mask;

    /*turn off complete bytes*/
    for (i = mask / BYTE_SIZE, j = IP_SIZE - 1; i > 0; --i, --j)
    {
        address[j] = 0;
    }
    /*turn off number of bits at end of specific byte*/
    i = mask % BYTE_SIZE;
    if (0 < i)
    {
        address[j] = address[j] & bit_mask[i];
    }
}

/*
void AddSubnet(IPAddress subnet, size_t mask, unsigned int added_part, IPAddress result)
{
}
*/

int IsValid(IPAddress subnet, const IPAddress address, size_t mask)
{
    size_t i = 0;
    size_t bytes_to_check = mask / BYTE_SIZE;
    size_t bits_to_check = mask % BYTE_SIZE;
    char bit_mask[8] = {0,          /*filler*/
                        (char)128,  /*1000 0000*/
                        (char)192,  /*1100 0000*/
                        (char)224,  /*1110 0000*/
                        (char)240,  /*1111 0000*/  
                        (char)248,  /*1111 1000*/             
                        (char)252,  /*1111 1100*/
                        (char)254}; /*1111 1110 - turn off one bit at end*/

    for (i = 0; i < bytes_to_check; ++i)
    {
        if ((subnet[i] & TURNED_ON_BYTE) != (address[i] & TURNED_ON_BYTE))
        {
            return 0;
        }
    }
    
    if (0 < bits_to_check)
    {
        if ((subnet[i] & bit_mask[bits_to_check]) != 
			(address[i] & bit_mask[bits_to_check]))
        {
            return 0;
        }
    }
    
    return 1;
}
