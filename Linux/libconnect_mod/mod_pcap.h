/*
 * mod_pcap.h
 *
 *  Created on: 2017-9-17
 *      Author: root
 */

#ifndef MOD_PCAP_H_
#define MOD_PCAP_H_

#if !(__linux__) && !(__WIN32__) && !(__APPLE__) && !(__CYGWIN__) && !(__GNU__)

#else   /* __linux__ */
#include <stdlib.h>


#endif  /* __linux__ */
#include <apr_optional.h>
#define PACKAGE_MAX_LEN 1024 //最大包长
#pragma pack(push,1) //开始定义数据包, 采用字节对齐方式
/*----------------------包头---------------------*/
typedef struct tagPACKAGEHEAD {
	unsigned char src_mac[6]; //源mac
	unsigned char dest_mac[6]; //目的mac

	unsigned short Command; //命令
	unsigned short nDataLen; //包体的长度
} PACKAGE_HEAD;
#pragma pack(pop) //结束定义数据包, 恢复原来对齐方式
#ifndef DFT_BUF_SIZE
#define DFT_BUF_SIZE 100
#endif

//FIXME:发包接口规划
/*
 * 作者：tecyang
 * 时间：2017-9-18
 * 方法名：sendPacket
 * 描述：发包函数
 * 参数:u_char* 发送数据
 * 返回值：void*
 */
void * sendPacket(u_char* data);

/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：pcap_Init
 * 描述：发包初始化函数
 * 参数:u_char* 发送数据
 * 返回值：int 状态码 正常为0
 */
//APR_DECLARE_OPTIONAL_FN(int,pcap_Init,(u_char *data));//
int pcap_Init(u_char *data);
/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：cust_package_head
 * 描述：构建包头
 * 参数:PACKAGE_HEAD* 自定义包头结构,unsigned short 自定义命令标识符, unsigned char[]源mac, unsigned char[]目的mac,  unsigned short 数据长度
 * 返回值：void
 */

void cust_package_head(PACKAGE_HEAD* head, unsigned short Command,
		unsigned char src_mac[6], unsigned char dest_mac[6],
		unsigned short nDataLen);
/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：cust_package
 * 描述：组包函数
 * 参数:unsigned char[] 包结构, PACKAGE_HEAD 包头, unsigned char* 包体
 * 返回值：void
 */
void cust_package(unsigned char package[], PACKAGE_HEAD head,
		unsigned char *body);
/*
 * 作者：root
 * 时间：2017-9-18
 * 方法名：read_post_data
 * 描述：从request中获取POST数据到缓存区
 * 参数:request_rec* apache request_rec 对象, char**  接收缓冲区, size_t* 接收缓冲区长度
 * 返回值：int
 */
int read_post_data(request_rec *req, char **post, size_t *post_size);

#endif /* MOD_PCAP_H_ */
