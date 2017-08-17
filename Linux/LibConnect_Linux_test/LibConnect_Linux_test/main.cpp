#include <stdio.h>
#include <libnet.h>
#include <string.h>
#include <pcap.h>
#include "Global.h"


char *device = NULL;//�豸���֣���ʱΪNULL
pcap_if_t *alldevs;
pcap_if_t *d;
libnet_t *l = NULL; // libnet context
char err_buf[LIBNET_ERRBUF_SIZE];
unsigned char src_mac[6];
unsigned char dest_mac[6];

//��ͬһ���������л�������ARP REPLY�����������ǣ�23.23.23.2��00:df:17:17:17:f2����
//#pragma comment(lib,"pcap.so")
//#pragma comment(lib,"ws2_32.so")
//#pragma comment(lib,"libnet.so")

void delay_50ms(unsigned int t)
{

	unsigned int j;
	for (;t>0;t--)
		for (j = 6245;j>0;j--)
			;
}

//����豸�������豸 -- ��ȡ������
int checkEth()
{

	int inum;
	int i = -ERROR_PCAP_FINDALLDEVS;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list */
	i = pcap_findalldevs(&alldevs, errbuf);
	if (i == ERROR_PCAP_FINDALLDEVS)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		return ERROR_PCAP_FINDALLDEVS;
	}

	device = alldevs[0].name;
	printf("select Net Device is %s", device);
	return i;
}
//void dispAllDevs()
//{
//    printf("Enter the interface number (1-%d):",i);
//	/* Print the list */
//    for(d=alldevs; d; d=d->next)
//    {
//        printf("%d. name: %s", ++i, d->name);
//        if (d->description)
//            printf("description (%s)\n", d->description);
//        else
//            printf(" (No description available)\n");
//    }
//}

/**��ʼ��*/
int cust_Init()
{
	//��ȡ��������
	if (checkEth()<0)
		return ERROR_INIT_FAIL;
	l = libnet_init(LIBNET_LINK_ADV, device, err_buf);
	return 1;
}
/**������ͷ*/
void cust_package_head(PACKAGE_HEAD* head, unsigned short Command, unsigned short nDataLen)
{

	PACKAGE_HEAD tag;
	
	tag.Command = Command;
	tag.nDataLen = nDataLen;
	*head = tag;
}
/**���*/
void cust_package(unsigned char package[], PACKAGE_HEAD head, unsigned char *body)
{
	//��ͷ
	int i = 2;
	package[0] = (unsigned char)head.Command;
	package[1] = (unsigned char)head.nDataLen;
	//����
	while (*(body + i) != '\0')
	{
		package[i] = *(body + i);
		i++;
	}



}
/**FIXME:���� ��ȡ�����ļ����д*/
int cust_link(unsigned char* _dstMac)
{
	//��������
	libnet_ptag_t p_tag;
	unsigned char data[] = { "1234567890" };
	PACKAGE_HEAD head;

	char* src_ip_str = { "169.254.190.252" };
	int res;
	int i = 0;
	unsigned long  dest_ip = 0, src_ip;
	unsigned char payload[PACKAGE_MAX_LEN];
	extern int loadConfigDemo(const char * str);
	int suc = 1;
	int fai = 0;
	//loadConfigDemo("./test.conf");
	//��ʼ������ͷ��
	cust_package_head(&head, 0x1, sizeof(data));
	//���
	cust_package(payload, head, data);
	//Ŀ��ipת��
	src_ip = libnet_name2addr4(l, src_ip_str, LIBNET_RESOLVE);
	//������ַ��ʼ��
	memcpy(src_mac, libnet_get_hwaddr(l)->ether_addr_octet, 6 * sizeof(unsigned char));
	memcpy(dest_mac, _dstMac, 6 * sizeof(unsigned char));
	//����ARP���ݰ�

	//p_tag = libnet_build_arp( // construct arp packet
	//    ARPHRD_ETHER, // hardware type ethernet  ARPHRD_ETHER
	//    ETHERTYPE_IP, // protocol type
	//    MAC_ADDR_LEN, // mac length
	//    IP_ADDR_LEN, // protocol length
	//    ARPOP_REPLY, // op type
	//    (u_int8_t*)src_mac, // source mac addr���������Ǹ���Ŀ�ĵص�ARP�� IP-MAC
	//    (u_int8_t*)&src_ip, // source ip addr
	//    (u_int8_t*)dest_mac, // dest mac addr
	//    (u_int8_t*)&dest_ip, // dest ip addr
	//    (u_int8_t*)&payload, // payload
	//    sizeof(payload), // payload length
	//    l, // libnet context
	//    0 //0 stands to build a new one
	//);
	//if(-1 == p_tag)
	//    printf("libnet_build_arp error\n"), exit(1);
	//��̫��ͷ��

	//������̫�����ݰ�
	p_tag = libnet_build_ethernet( // create ethernet header
		(u_int8_t*)dest_mac, // dest mac addr
		(u_int8_t*)src_mac, // source mac addr����˵������·���ԴMAC��ַ��������˿���αװ�Լ�
		0x2222, // protocol type ETHERTYPE_ARP
		payload, // payload
		sizeof(payload), // payload length
		l, // libnet context
		0 // 0 to build a new one
	);
	if (-1 == p_tag)
		printf("libnet_build_ethernet error!\n"), exit(1);

	if (-1 == (res = libnet_write(l)))
		printf("libnet_write error!\n"), exit(1);


	while(true){
		delay_50ms(200);
		
		if (-1 == (res = libnet_write(l)))
		{
			fai++;

			FILE *fp = fopen("send_error_log.txt", "w");
			fprintf(fp, "send error!%d\n", fai);
			fclose(fp);

			//printf("send error!%d\n",fai);
		}
		else {

			FILE *fp = fopen("send_suc_log.txt", "w");
			fprintf(fp, "send successfully!%d\n", suc);
			fclose(fp);
			//printf("send successfully!%d\n", suc);
			suc++;
		}
	
	}
		
}
/**�ͷ�����*/
void cust_destroy()
{
	libnet_destroy(l);
}

int main(int argc, char *argv[])
{
	/*unsigned char dest_mac[MAC_ADDR_LEN]
		= { 0xb8, 0x27, 0xeb, 0x72, 0x8d, 0xb2 };*/
	unsigned char dest_mac[MAC_ADDR_LEN]
		= { 0xb8, 0x27, 0xeb, 0xc7, 0x79, 0xa5 };


	/*unsigned char dest_mac[MAC_ADDR_LEN]
	= {0x00, 0x0c, 0x29, 0x56, 0x43, 0x9f};*/
	//cust_Init();
	//��ȡ��������
	if (checkEth()<0)
		return ERROR_INIT_FAIL;
	l = libnet_init(LIBNET_LINK_ADV, device, err_buf);
	//return 1;
	cust_link(dest_mac);

	cust_destroy();
	/*
	libnet_ptag_t p_tag;
	//src_mac = {0x28, 0xd2, 0x44, 0x18, 0xfc, 0xfe};
	unsigned char dest_mac[MAC_ADDR_LEN]
	= {0xb8, 0x27, 0xeb, 0x72, 0x8d, 0xb2};
	char *src_ip_str = "192.168.137.1";
	unsigned char payload[] = "����ͨѶ����";
	unsigned long src_ip, dest_ip = 0;
	int res;
	src_ip = libnet_name2addr4(l, src_ip_str, LIBNET_RESOLVE);

	// create libnet environment

	l = libnet_init(LIBNET_LINK_ADV, device, err_buf);
	if(!l)
	printf("libnet_init error\n"), exit(1);
	//����ARP���ݰ�

	p_tag = libnet_build_arp( // construct arp packet
	ARPHRD_ETHER, // hardware type ethernet
	ETHERTYPE_IP, // protocol type
	MAC_ADDR_LEN, // mac length
	IP_ADDR_LEN, // protocol length
	ARPOP_REPLY, // op type
	(u_int8_t*)src_mac, // source mac addr���������Ǹ���Ŀ�ĵص�ARP�� IP-MAC
	(u_int8_t*)&src_ip, // source ip addr
	(u_int8_t*)dest_mac, // dest mac addr
	(u_int8_t*)&dest_ip, // dest ip addr
	(u_int8_t*)&payload, // payload
	sizeof(payload), // payload length
	l, // libnet context
	0 //0 stands to build a new one
	);
	if(-1 == p_tag)
	printf("libnet_build_arp error\n"), exit(1);
	//��̫��ͷ��

	p_tag = libnet_build_ethernet( // create ethernet header
	(u_int8_t*)dest_mac, // dest mac addr
	(u_int8_t*)src_mac, // source mac addr����˵������·���ԴMAC��ַ��������˿���αװ�Լ�
	ETHERTYPE_ARP, // protocol type
	NULL, // payload
	0, // payload length
	l, // libnet context
	0 // 0 to build a new one
	);
	if(-1 == p_tag)
	printf("libnet_build_ethernet error!\n"), exit(1);


	if(-1 == (res = libnet_write(l)))
	printf("libnet_write error!\n"), exit(1);
	libnet_destroy(l);
	*/
	return 0;
}
