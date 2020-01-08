/****************************************************************************
*                                                                           *
*                               OL79 - 02/01/19                             *
*                                                                           *
*                                 Ip										*
*                                                                           *
*****************************************************************************/

#ifndef ILRD_IP_H
#define ILRD_IP_H

#include <stddef.h> /* size_t */

#define IP_SIZE (4)

typedef unsigned char IPAddress[IP_SIZE];
typedef unsigned int BinaryIp;

/* Convert unsigned int address to binary */
BinaryIp IPtoBit(const IPAddress address);

/* Convert binary address to unsigned int address. Then places it in result_ip */
void BitToIp(BinaryIp ip, IPAddress result_ip);

/* cut the mask out of the subnet. 
   Update result in address */
void CutSubnet(IPAddress address, size_t mask);

/* add added_part to subnet using the mask. 
   Update result in subnet */
void AddSubnet(IPAddress subnet, size_t mask, unsigned int added_part, IPAddress result);

/* determines if address is valid compared to subnet */
int IsValid(IPAddress subnet, const IPAddress address, size_t mask);




#endif
