#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tec_pcap.h"

int main() {

	char errBuf[PCAP_ERRBUF_SIZE], *devStr;

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

	if (!device) {
		printf("error: pcap_open_live(): %s\n", errBuf);
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
	pcap_loop(device, -1, getPacket, (u_char*) &id);

	pcap_close(device);

	return 0;
}

