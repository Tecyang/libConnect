/*
**  mod_libconnect.c -- Apache sample libconnect module
**  [Autogenerated via ``apxs -n libconnect -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory
**  by running,0x
**
**    $ apxs -c -i mod_libconnect.c
**		apxs -c -i -a mod_libconnect.c libapr-1.lib libaprutil-1.lib libapriconv-1.lib libhttpd.lib
**  Then activate it in Apache's httpd.conf file for instance
**  for the URL /libconnect in as follows,0x
**
**    #   httpd.conf
**    LoadModule libconnect_module modules/mod_libconnect.so
**    <Location /libconnect>
**    SetHandler libconnect
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /libconnect and watch for the
**  output of this module. This can be achieved for instance via,0x
**
**    $ lynx -mime_header http,0x//localhost/libconnect
**
**  The output should be similar to the following one,0x
**
**    HTTP/1.1 200 OK
**    Date,0x Tue, 31 Mar 1998 14,0x42,0x22 GMT
**    Server,0x Apache/1.3.4 (Unix)
**    Connection,0x close
**    Content-Type,0x text/html
**
**    The sample page from mod_libconnect.c
*/

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "http_log.h"

//#include <apr_errno.h>

#include <string.h>
#include <stdio.h>
#include <libnet.h>
#include <windows.h>
//#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Global.h"


#pragma comment(lib,"wpcap.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libnet.lib")
#pragma comment(lib,"pthreadVC2.lib")

#pragma comment(lib,"apr-1.lib")
#pragma comment(lib,"libapr-1.lib")
#pragma comment(lib,"aprutil-1.lib")
#pragma comment(lib,"libhttpd.lib")


char *device = NULL;//设备名字，此时为NULL
pcap_if_t *alldevs;
//pcap_if_t *d;
libnet_t *l = NULL; // libnet context
char err_buf[LIBNET_ERRBUF_SIZE];
unsigned char src_mac[6];
unsigned char dest_mac[6];
request_rec *rec;
struct pcap_pkthdr *pktheader;
//
//pthread_t thread[2];//线程函数返回类型
//pthread_mutex_t mut;//线程互斥锁类型

apr_thread_t *thread_t = NULL;//线程
apr_threadattr_t *threadattr_t = NULL;//线程属性
apr_pool_t *pool_t = NULL;//线程内存池
apr_status_t rv;//接收返回值
apr_thread_mutex_t *thread_mutex_t;


struct ether_header
{
unsigned char ether_dhost[6];   //目的mac
unsigned char ether_shost[6];   //源mac
unsigned short ether_type;      //以太网类型
};

void delay_50ms(unsigned int t)
{

	unsigned int j;
	for (;t>0;t--)
		for (j = 6245;j>0;j--)
			;
}

//检测设备中网卡设备 -- 获取的数量
int checkEth()
{
//	fprintf(stderr, "checkEth");
	int inum;
	int i = -ERROR_PCAP_FINDALLDEVS;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list */
	i = pcap_findalldevs(&alldevs, errbuf);
	if (i == ERROR_PCAP_FINDALLDEVS)
	{
	//	fprintf(stderr, "Error in pcap_findalldevs,0x %s\n", errbuf);
		return ERROR_PCAP_FINDALLDEVS;
	}

	device = "\\Device\\NPF_{2170E295-1C69-44AD-BA37-B3497BA1337E}";//alldevs[4].name;
	return i;

}

/**初始化*/

int cust_Init()
{
	//获取网卡出错
	if (checkEth()<0)
		return ERROR_INIT_FAIL;
	l = libnet_init(LIBNET_LINK_ADV, device, err_buf);
	return 1;
}
/**构建包头*/
void cust_package_head(PACKAGE_HEAD* head, unsigned short Command, unsigned short nDataLen)
{

	PACKAGE_HEAD tag;
	tag.Command = Command;
	tag.nDataLen = nDataLen;
	*head = tag;
}
/**组包*/
void cust_package(unsigned char *package, PACKAGE_HEAD head, unsigned char *body)
{

	strcpy(package, &head.Command);
	strcat(package, &head.nDataLen);
	
	//包头

	strcat(package, body);
}

//发包线程函数1
static void* APR_THREAD_FUNC thread1(apr_thread_t *th,void *data)
{
	//apr_thread_mutex_lock(thread_mutex_t);
	int th_num = 0;
	int res = 0;
	for (th_num = 0;th_num < 1000;th_num++)
	{
		if (-1 == (res = libnet_write(l)))
		{
	
		/*	ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, rec,
				"libnet_write error!\n");*/
			exit(1);
	
		}
	}
	apr_status_t rv = NULL;
	apr_thread_exit(th, rv);
	//apr_thread_mutex_unlock(thread_mutex_t);
}


//TODO,0x连接 读取配置文件需编写*/
//ip.src == 28,0xd2,0x44,0x18,0xfc,0xfe
int cust_link(unsigned char* _dstMac, char **tran_data)
{
	//变量定义
	libnet_ptag_t p_tag;
	//TODO,0x发包数据隔离
	/*unsigned char *data = (char *)malloc(sizeof(char)*PACKAGE_MAX_LEN);
	size_t data_size = PACKAGE_MAX_LEN;

	if (data == NULL) {
		return ERROR_DATA_LENGTH;
	}

	memset(data, '\0', data_size);*/

	//unsigned char data[] = { *tran_data };
	PACKAGE_HEAD head;

	char* src_ip_str = { "192.168.191.1" };
	int res;
	int i = 0;
	unsigned long  dest_ip = 0, src_ip;

	//extern int loadConfigDemo(const char * str);
	unsigned char *payload = (unsigned char *)malloc(2 * sizeof(WORD) + strlen(*tran_data));

	//loadConfigDemo("./test.conf");
	//初始化发包头部
	cust_package_head(&head, 0x1, strlen(*tran_data));


	//组包
	cust_package(payload, head, *tran_data);
	//目标ip转换
	src_ip = libnet_name2addr4(l, src_ip_str, LIBNET_RESOLVE);
	//网卡地址初始化
	memcpy(src_mac, libnet_get_hwaddr(l)->ether_addr_octet, 6 * sizeof(unsigned char));
	memcpy(dest_mac, _dstMac, 6 * sizeof(unsigned char));
	//构造ARP数据包


	//p_tag = libnet_build_arp( // construct arp packet
	//    ARPHRD_ETHER, // hardware type ethernet  ARPHRD_ETHER
	//    ETHERTYPE_IP, // protocol type
	//    MAC_ADDR_LEN, // mac length
	//    IP_ADDR_LEN, // protocol length
	//    ARPOP_REPLY, // op type
	//    (u_int8_t*)src_mac, // source mac addr这里作用是更新目的地的ARP表 IP-MAC
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
	//以太网头部


	//构造以太网数据包
	p_tag = libnet_build_ethernet( // create ethernet header
		(u_int8_t*)dest_mac, // dest mac addr
		(u_int8_t*)src_mac, // source mac addr这里说明你链路层的源MAC地址，如果改了可以伪装自己
		0x2222, // protocol type ETHERTYPE_ARP
		payload, // payload
		strlen(payload), // payload length
		l, // libnet context
		0 // 0 to build a new one
	);
	if (-1 == p_tag)
		printf("libnet_build_ethernet error!\n"), exit(1);

	if (-1 == (res = libnet_write(l)))
		printf("libnet_write error!\n"), exit(1);
	//TODO:需要进行发包频率控制
	//考虑多线程 接收端一样
	//TODO:TOP1线程创建完成，需要进行加锁，多个线程共同发送，主线程进行锁定测试发包速度
	
	char* buff;//初始内存大小
	//rv = apr_initialize();//apr初始化
	//rv = apr_pool_create(&pool_t, NULL);//创建内存池
	//buff = apr_palloc(pool_t,APR_MEMNODE_T_SIZE);//分配初始内存
	////apr_threadattr_create(threadattr_t, pool_t);
	////创建线程
	//if ((rv = apr_thread_create(&thread_t, NULL,  thread1, NULL, pool_t)) != APR_SUCCESS)
	//{
	//	return -1;
	//}

//	rv =apr_thread_mutex_trylock(*thread_mutex_t);
	

	
	
	for (i = 0;i < 1000000;i++)
	{
		//进行发包
		if (-1 == (res = libnet_write(l)))
		{

			ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, rec,
				"libnet_write error!\n");
			exit(1);
			//		printf("libnet_write error!\n"), exit(1);

		}
	}

	

	return 0;

}
/**释放连接*/
void cust_destroy()
{
	libnet_destroy(l);
}

int testmain(char **post)
{
	
	//unsigned char dest_mac[MAC_ADDR_LEN]
	//	= { 0xb8, 0x27, 0xeb, 0x72, 0x8d, 0xb2 };
	unsigned char dest_mac[MAC_ADDR_LEN]
		= { 0xb8,0x27 ,0xeb ,0x92 ,0x2c ,0xf0 };
	char errBuf[PCAP_ERRBUF_SIZE];

	//获取本机网卡设备
	//TODO,0x需要进行可配置的网卡获取
	if (checkEth()<0)
		return ERROR_INIT_FAIL;
	//ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, rec,
	//	"Comming testmain\n");

	/* open a device, wait until a packet arrives */
	l = libnet_init(LIBNET_LINK_ADV, device, errBuf);
	
	//return 1;
	cust_link(dest_mac,post);

	cust_destroy();
	return 0;
}

/**
* @brief read_post_data 从 request 中获取 POST 数据到缓冲区
*
* @param req         apache request_rec 对象
* @param post        接收缓冲区
* @param post_size   接收缓冲区长度
*
* @return
*/
static int read_post_data(request_rec *req, char **post, size_t *post_size) {
	char buffer[DFT_BUF_SIZE] = { 0 };
	size_t bytes, count, offset;

	bytes = count = offset = 0;

	if (ap_setup_client_block(req, REQUEST_CHUNKED_DECHUNK) != OK) {
		return HTTP_BAD_REQUEST;
	}

	if (ap_should_client_block(req)) {
		for (bytes = ap_get_client_block(req, buffer, DFT_BUF_SIZE);
			bytes > 0;
			bytes = ap_get_client_block(req, buffer, DFT_BUF_SIZE)) {
			count += bytes;
			if (count > *post_size) {
				*post = (char *)realloc(*post, count);
				if (*post == NULL) {
					return HTTP_INTERNAL_SERVER_ERROR;
				}
			}
			*post_size = count;
			offset = count - bytes;
			memcpy((char *)*post + offset, buffer, bytes);
		}
	}
	else {
		*post_size = 0;
		return OK;
	}

	return OK;
}

/* The sample content handler */
static int libconnect_handler(request_rec *r)
{


	if (strcmp(r->handler, "libconnect")) {
		return DECLINED;
	}

	if ((r->method_number != M_GET) && (r->method_number != M_POST)) {
		return HTTP_METHOD_NOT_ALLOWED;
	}

	char *post = (char *)malloc(sizeof(char)*DFT_BUF_SIZE);
	size_t post_size = DFT_BUF_SIZE;

	if (post == NULL) {
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	memset(post, '\0', post_size);

	int ret = read_post_data(r, &post, &post_size);
	//post 为接收到的post过来的数据

	//进行数据验证
	if (ret != OK) {
		free(post);
		post = NULL;
		post_size = 0;
		return ret;
	}
	/*apr_thread_t **m_thread;
	apr_threadattr_t *m_threadattr;
	apr_pool_t *m_apr_pool_count;
	apr_thread_start_t apr_func;
*/
	//apr_pool_create(m_apr_pool_count, NULL);
	//apr_status_t apr_status;
	//m_apr_pool_count = 0;

	//验证通过 进行数据传输
	//apr_status = apr_thread_create(&m_thread, NULL, apr_func, NULL,m_apr_pool_count);
	testmain(&post);
// a) 输出参数为第一个参数；
//	b) 如果某个函数需要内部分配内存，则将一个apr_pool_t参数放在最后
	ap_set_content_type(r, "text/html;charset=utf-8");
	ap_set_content_length(r, post_size);

	if (post_size == 0) {
		ap_rputs("no post data found", r);
		return OK;
	}

	ap_rputs(post, r);

	free(post);
	post = NULL;
	post_size = 0;

	return OK;
}

static void libconnect_register_hooks(apr_pool_t *p)
{
	ap_hook_handler(libconnect_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA libconnect_module = {
	STANDARD20_MODULE_STUFF,
	NULL,                  /* create per-dir    config structures */
	NULL,                  /* merge  per-dir    config structures */
	NULL,                  /* create per-server config structures */
	NULL,                  /* merge  per-server config structures */
	NULL,                  /* table of config file commands       */
	libconnect_register_hooks  /* register hooks                      */
};


