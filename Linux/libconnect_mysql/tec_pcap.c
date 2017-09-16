/*
 * tec_pcap.c
 *
 *  Created on: 2017-9-16
 *      Author: root
 */
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tec_pcap.h"
#include "tec_mysql.h"

struct ether_header {
	unsigned char ether_dhost[6]; //目的mac
	unsigned char ether_shost[6]; //源mac
	unsigned short ether_type; //以太网类型
};

/* mysql */
MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;
char *query;
int flag, t;
bool sqlLinked = false;

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr,
		const unsigned char *packet_content) {
	unsigned char *mac_string; //
	struct ether_header *ethernet_protocol;
	unsigned short ethernet_type; //以太网类型
	ethernet_protocol = (struct ether_header *) packet_content;
	//if(ntohs(ethernet_protocol->ether_type)==0x2222){
	mac_string = (unsigned char *) ethernet_protocol->ether_shost; //获取源mac地址
	printf("Mac Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	mac_string = (unsigned char *) ethernet_protocol->ether_dhost; //获取目的mac
   	printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	ethernet_type = ethernet_protocol->ether_type; //获得以太网的类型
  	printf("Ethernet type is :%04x\n",ethernet_type);
	switch (ethernet_type) {
	case 0x0800:
		printf("The network layer is IP protocol\n");
		break; //ip
	case 0x0806:
		printf("The network layer is ARP protocol\n");
		break; //arp
	case 0x0835:
		printf("The network layer is RARP protocol\n");
		break; //rarp
	default:
		break; //printf("protocol type is %04x\n",ethernet_type);break;
	}
	int * id = (int *) arg;
	int * id2 = (int *) arg;
	if (ethernet_type == 0x2222) {
		if (pkthdr->len == 1038) {
			++(*id);
			FILE *fp = fopen("rec_suc_log.txt", "w");
			fprintf(fp, "rec count!%d\n", *id);
			fclose(fp);
		} else {

			++(*id2);
			FILE *fp = fopen("rec_error_log.txt", "w");
			fprintf(fp, "rec count!%d\n", *id2);
			fclose(fp);
		}

		printf("protocol type is %04x",ethernet_type);
		printf("id: %d\n", ++(*id));
		printf("Packet length: %d\n", pkthdr->len);
		printf("Number of bytes: %d\n", pkthdr->caplen);
		printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));
		int i;

		/*for(i=0; i<pkthdr->len; ++i)
		 {
		 printf(" %02x", packet_content[i]);
		 if( (i + 1) % 16 == 0 )
		 {
		 printf("\n");
		 }
		 }
		 printf("\n\n");*/

		if (!sqlLinked) {
			sqlLinked = sql_init(&mysql);
			if (sqlLinked) {

				query = "select * from test";
				/*查询，成功则返回0*/
				flag = mysql_real_query(&mysql, query,
						(unsigned int) strlen(query));
				if (flag) {
					printf("Query failed!\n");

				} else {
					printf("[%s] made...\n", query);
				}

				/*mysql_store_result讲全部的查询结果读取到客户端*/
				res = mysql_store_result(&mysql);

			} else {
				printf("mysql init false");

			}

			/*mysql_fetch_row检索结果集的下一行*/
			while (row = mysql_fetch_row(res)) {
				/*mysql_num_fields返回结果集中的字段数目*/
				for (t = 0; t < mysql_num_fields(res); t++) {
					printf("fields %d\t", t);
				}
				printf("\n");
			}
			mysql_close(&mysql);

		}

	}

}

