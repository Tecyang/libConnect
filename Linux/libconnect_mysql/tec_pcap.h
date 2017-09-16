/*
 * tec_pcap.h
 *
 *  Created on: 2017-9-16
 *      Author: root
 */

#ifndef TEC_PCAP_H_
#define TEC_PCAP_H_

#include <pcap.h>
void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const unsigned char *packet_content);


#endif /* TEC_PCAP_H_ */
