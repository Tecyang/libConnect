#include <pcap.h>
#include <httpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <pthread.h>
#include "ap_config.h"
#include "http_protocol.h"
#include "mod_pcap.h"
#include <apr_optional.h>
//
//u_char a[1024] = { 0x00, 0xd0, 0xd0, 0xa1, 0x1f, 0x40, 0x00, 0x10, 0xdb, 0x3f,
//		0xa1, 0x35, 0x08, 0x00, 0x45, 0x00, 0x00, 0x80, 0x95, 0x11, 0x00, 0x00,
//		0x3f, 0x01, 0x8b, 0x5b, 0xc2, 0x88, 0x51, 0xde, 0x46, 0x00, 0x00, 0xaa,
//		0x00, 0x00, 0x32, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
//		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
//		31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
//		49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
//		67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
//		85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, };

pcap_t* descr; //libpcap 指针
char *dev; //设备名 linux 默认为eth0
char errbuf[PCAP_ERRBUF_SIZE]; //错误信息
unsigned char src_mac[6] = { };
unsigned char dest_mac[6] = { };
PACKAGE_HEAD head; //包头

/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：read_post_data
 * 描述：从request中获取POST数据到缓存区
 * 参数:request_rec* apache request_rec 对象, char**  接收缓冲区, size_t* 接收缓冲区长度
 * 返回值：int
 */
int read_post_data(request_rec *req, char **post, size_t *post_size) {
	char buffer[DFT_BUF_SIZE] = { 0 };
	size_t bytes, count, offset;

	bytes = count = offset = 0;

	if (ap_setup_client_block(req, REQUEST_CHUNKED_DECHUNK) != OK) {
		return HTTP_BAD_REQUEST;
	}

	if (ap_should_client_block(req)) {
		for (bytes = ap_get_client_block(req, buffer, DFT_BUF_SIZE); bytes > 0;
				bytes = ap_get_client_block(req, buffer, DFT_BUF_SIZE)) {
			count += bytes;
			if (count > *post_size) {
				*post = (char *) realloc(*post, count);
				if (*post == NULL) {
					return HTTP_INTERNAL_SERVER_ERROR;
				}
			}
			*post_size = count;
			offset = count - bytes;
			memcpy((char *) *post + offset, buffer, bytes);
		}
	} else {
		*post_size = 0;
		return OK;
	}

	return OK;
}

/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：pcap_Init
 * 描述：发包初始化函数
 * 参数:u_char* 发送数据
 * 返回值：int 状态码 正常为0
 */
int pcap_Init(u_char *data) {
//	int i;
//
//	const u_char *packet;
//	struct pcap_pkthdr hdr; /* pcap.h                    */
//	struct ether_header *eptr; /* net/ethernet.h            */
	struct bpf_program fp; /* hold compiled program     */
	bpf_u_int32 maskp; /* subnet mask               */
	bpf_u_int32 netp; /* ip                        */

	pthread_t send_t;
	pthread_t send_t1;
	pthread_attr_t * attr = NULL;
	unsigned char payload[PACKAGE_MAX_LEN];
	/* grab a device to peak into... */
	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "%s\n", errbuf);
		exit(1);
	}

	/* ask pcap for the network address and mask of the device */
	pcap_lookupnet(dev, &netp, &maskp, errbuf);

	//XXX 以下代码需要修改 在apache模块中不应该使用exit，这将会导致整个服务崩溃退出 2017-09-21 tecyang
	/* open device for reading this time lets set it in promiscuous
	 * mode so we can monitor traffic to another machine             */
	descr = pcap_open_live(dev, BUFSIZ, 1, -1, errbuf);
	if (descr == NULL) {
		printf("pcap_open_live(): %s\n", errbuf);
		exit(1);
	} else {
		printf("open_live successfully");
	}
	/* Lets try and compile the program.. non-optimized */
	if (pcap_compile(descr, &fp, "host 130.0.0", 0, netp) == -1) {
		fprintf(stderr, "Error calling pcap_compile\n");
		exit(1);
	}

	/* set the compiled program as the filter */
	if (pcap_setfilter(descr, &fp) == -1) {
		fprintf(stderr, "Error setting filter\n");
		exit(1);
	}

	//:构建包头
	cust_package_head(&head, 0x1, src_mac, dest_mac,
			(unsigned short) strlen(data));
	//:组包
	cust_package(payload, head, data);
	int **ret;
	int num1 = 1;
	int num2 = 2;
	//开启线程进行发包--测试方法
	pthread_create(&send_t, attr, (void *)sendPacket, payload);
	//pthread_create(&send_t1, attr, sendPacket, &num2);

	pthread_join(send_t, (void**) ret);
	//pthread_join(send_t1, (void**) ret);
	//  printf("thread1 return %d",**ret);

	return 0;
}

/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：cust_package_head
 * 描述：构建包头
 * 参数:PACKAGE_HEAD* 自定义包头结构, unsigned short 自定义命令标识符,unsigned char[]源mac, unsigned char[]目的mac,  unsigned short 数据长度
 * 返回值：void
 */

void cust_package_head(PACKAGE_HEAD* head, unsigned short Command,
		unsigned char src_mac[6], unsigned char dest_mac[6],
		unsigned short nDataLen) {

	PACKAGE_HEAD tag;
	memcpy(tag.src_mac,src_mac,6*sizeof(unsigned char));
	memcpy(tag.dest_mac,dest_mac,6*sizeof(unsigned char));

	tag.Command = Command;
	tag.nDataLen = nDataLen;
	*head = tag;
}

/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：cust_package
 * 描述：组包函数
 * 参数:unsigned char[] 包结构, PACKAGE_HEAD 包头, unsigned char* 包体
 * 返回值：void
 */
void cust_package(unsigned char package[], PACKAGE_HEAD head,
		unsigned char *body) {
	//包头
	int i = 14;
	memcpy(&package, &head.src_mac, 6 * sizeof(unsigned char));
	memcpy(&package, &head.dest_mac, 6 * sizeof(unsigned char));
	package[0] = (unsigned char) head.Command;
	package[1] = (unsigned char) head.nDataLen;
	//包体
	while (*(body + i) != '\0') {
		package[i] = *(body + i);
		i++;
	}

}
/*
 * 作者：tecyang
 * 时间：2017-9-18
 * 方法名：sendPacket
 * 描述：发包函数
 * 参数:u_char* 发送数据
 * 返回值：void*
 */
void * sendPacket(u_char* data) {
//	int i = 0;
//	for (i = 0; i < 1000; ++i) {
	pcap_sendpacket(descr, data, strlen(data));
	printf("send packet THREAD %d", *(char *) data);
//	}

}
//static void pcap_hooks(apr_pool_t *pool)
//{
//	apr_hook_handler(pcap_handler,NULL,NULL,APR_HOOK_MIDDLE);
//}
