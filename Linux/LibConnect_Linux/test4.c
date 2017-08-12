#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
struct ether_header  
{  
    unsigned char ether_dhost[6];   //目的mac  
    unsigned char ether_shost[6];   //源mac  
    unsigned short ether_type;      //以太网类型  
};  
void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const unsigned char *packet_content)
{
	unsigned char *mac_string;              //
	struct ether_header *ethernet_protocol;
	unsigned short ethernet_type;           //以太网类型
  	ethernet_protocol = (struct ether_header *)packet_content;  
	//if(ntohs(ethernet_protocol->ether_type)==0x2222){ 
   	mac_string = (unsigned char *)ethernet_protocol->ether_shost;//获取源mac地址
//	printf("Mac Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
   	mac_string = (unsigned char *)ethernet_protocol->ether_dhost;//获取目的mac
//   	printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
    	ethernet_type = ethernet_protocol->ether_type;//获得以太网的类型
//  	printf("Ethernet type is :%04x\n",ethernet_type);  
    	switch(ethernet_type)
    	{
		case 0x0800:printf("The network layer is IP protocol\n");break;//ip  
      		case 0x0806:printf("The network layer is ARP protocol\n");break;//arp  
       	 	case 0x0835:printf("The network layer is RARP protocol\n");break;//rarp  
       		default:break;//printf("protocol type is %04x\n",ethernet_type);break;
    	}
	int * id = (int *)arg;
	int * id2 = (int *)arg;
  	if(ethernet_type==0x2222){
		if (pkthdr->len == 1038)
		{
			++(*id);
			FILE *fp = fopen("rec_suc_log.txt", "w");
			fprintf(fp, "rec count!%d\n", *id);
			fclose(fp);
		}
		else
		{
		
			++(*id2);
			FILE *fp = fopen("rec_error_log.txt", "w");
			fprintf(fp, "rec count!%d\n", *id2);
			fclose(fp);
		}

		//printf("protocol type is %04x",ethernet_type);
  		//printf("id: %d\n", ++(*id));
  		//printf("Packet length: %d\n", pkthdr->len);
 		//printf("Number of bytes: %d\n", pkthdr->caplen);
  		//printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));
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

	}

}

int main()
{
  char errBuf[PCAP_ERRBUF_SIZE], * devStr;
  
  /* get a device */
  /*devStr = pcap_lookupdev(errBuf);
  
  if(devStr)
  {
    printf("success: device: %s\n", devStr);
  }
  else
  {
    printf("error: %s\n", errBuf);
    exit(1);
  }
  */
  /* open a device, wait until a packet arrives */
  pcap_t * device = pcap_open_live("eth0", 65535, 1, 0, errBuf);
  
  if(!device)
  {
  //  printf("error: pcap_open_live(): %s\n", errBuf);
    exit(1);
  }
 printf("succ:pcap_open_live()\n");

/* construct a filter */
  
  struct bpf_program filter;
//  pcap_compile(device, &filter, "icmp[icmptype] == icmp-echoreply or icmp[icmptype] == icmp-echo", 1, 0);
 // pcap_compile(devic:e, &filter, "ip[8] == 5", 1, 0);
 //char* filter_str = "ehter dst b8:27:eb:72:8d:b2";
 //pcap_compile(device,&filter,filter_str,1,0);
 // pcap_compile(device,&filter,"arp",1,0);
 
//  printf("pcap filter is %s \n",filter_str);

  pcap_setfilter(device, &filter);
  
  /* wait loop forever */
  int id = 0;
  pcap_loop(device, -1, getPacket, (u_char*)&id);

  
  pcap_close(device);

  return 0;
}

