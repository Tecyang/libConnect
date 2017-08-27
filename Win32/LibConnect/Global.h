
#ifndef MAC_ADDR_LEN
#define MAC_ADDR_LEN 6
#endif

#ifndef IP_ADDR_LEN
#define IP_ADDR_LEN 4
#endif



#define ERROR_PCAP_FINDALLDEVS -1 //Error in pcap_findalldevs
#define ERROR_PCAP_NOTINSTALL -2 //Error winpcap not install
#define ERROR_INIT_FAIL -3 //init function fail


#define PACKAGE_MAX_LEN 1024 //������

#ifndef DFT_BUF_SIZE
#define DFT_BUF_SIZE 100
#endif

#pragma pack(push,1) //��ʼ�������ݰ�, �����ֽڶ��뷽ʽ
/*----------------------��ͷ---------------------*/
typedef struct tagPACKAGEHEAD
{
 WORD Command; //���� 
 WORD nDataLen;//����ĳ���
}PACKAGE_HEAD;
#pragma pack(pop) //�����������ݰ�, �ָ�ԭ�����뷽ʽ
