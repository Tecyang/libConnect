#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <pthread.h>
#include "test.h"

u_char a[1024] = {0x00,0xd0,0xd0,0xa1,0x1f,0x40,0x00,0x10,0xdb,0x3f,0xa1,0x35,0x08,0x00,\
                  0x45,0x00,0x00,0x80,0x95,0x11,0x00,0x00,0x3f,0x01,0x8b,0x5b,0xc2,0x88,0x51,0xde,\
                  0x46,0x00,0x00,0xaa,0x00,0x00,0x32,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,\
                  11,12,13,14,15,16,17,18,19,20,\
                  21,22,23,24,25,26,27,28,29,30,\
                  31,32,33,34,35,36,37,38,39,40,\
                  41,42,43,44,45,46,47,48,49,50,\
                  51,52,53,54,55,56,57,58,59,60,\
                  61,62,63,64,65,66,67,68,69,70,\
                  71,72,73,74,75,76,77,78,79,80,\
                  81,82,83,84,85,86,87,88,89,90,\
                  91,92,93,94,95,96,97,98,99,100,
                 };
 pcap_t* descr;

int main(int argc,char **argv)
{
    int i;
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];

    const u_char *packet;
    struct pcap_pkthdr hdr;     /* pcap.h                    */
    struct ether_header *eptr;  /* net/ethernet.h            */
    struct bpf_program fp;      /* hold compiled program     */
    bpf_u_int32 maskp;          /* subnet mask               */
    bpf_u_int32 netp;           /* ip                        */


    pthread_t  send_t;
    pthread_t  send_t1;
    pthread_attr_t * attr= NULL;

    /* grab a device to peak into... */
    dev = pcap_lookupdev(errbuf);
    if(dev == NULL)
    {
        fprintf(stderr,"%s\n",errbuf);
        exit(1);
    }

    /* ask pcap for the network address and mask of the device */
    pcap_lookupnet(dev,&netp,&maskp,errbuf);

    /* open device for reading this time lets set it in promiscuous
     * mode so we can monitor traffic to another machine             */
    descr = pcap_open_live(dev,BUFSIZ,1,-1,errbuf);
    if(descr == NULL)
    {
        printf("pcap_open_live(): %s\n",errbuf);
        exit(1);
    }
    else
    {
        printf("open_live successfully");
    }
    /* Lets try and compile the program.. non-optimized */
    if(pcap_compile(descr,&fp,"host 130.0.0",0,netp) == -1)
    {
        fprintf(stderr,"Error calling pcap_compile\n");
        exit(1);
    }

    /* set the compiled program as the filter */
    if(pcap_setfilter(descr,&fp) == -1)
    {
        fprintf(stderr,"Error setting filter\n");
        exit(1);
    }
    int **ret;
    int num1 = 1;
    int num2 = 2;
    pthread_create(&send_t,attr,sendPacket,&num1);
    pthread_create(&send_t1,attr,sendPacket,&num2);
    pthread_join(send_t, (void**)ret);
    pthread_join(send_t1, (void**)ret);
  //  printf("thread1 return %d",**ret);


    return 0;
}

 void * sendPacket(void* num)
{
    int i = 0;
     for(i = 0; i< 1000000; ++i)
    {
        pcap_sendpacket(descr, a,1024) ;
        printf("send packet THREAD %d",*(int *)num);
    }

}
