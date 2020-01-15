#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../packet.h"
#include <stdio.h>
#include "error.h"

#define SRC "192.168.1.2"
#define DST "127.0.0.1"

int main (void) {

	struct packet pkt;
	struct pkt_socket sck;
	unsigned short int mss;
	
	fprintf(stdout, "1\n");
	pkt_init(&pkt, PKT_NET_IP | PKT_TRANS_UDP, 28);
	fprintf(stdout, "2\n");
	pkt_ip_header(&pkt, 5, 4, 0, 28, 0, 0, 255, IPPROTO_UDP, 0, 
			inet_addr(SRC), inet_addr(DST));
	fprintf(stdout, "3\n");
	pkt_move_actptr(&pkt, 20);
	pkt_udp_header(&pkt, 53, 64324, 8, 0);
	pkt_udp_cksum(&pkt, SRC, DST, 8);
	if (pkt_socket_open(&sck, PKT_RAW) < 0) {
		fprintf(stdout, "4autsch\n");
		err_sys("&pkt_send");
	}
	pkt_socket_prepare(&sck, DST);
	fprintf(stdout, "4\n");
	if (pkt_send(&pkt, &sck) < 0) {
		//fprintf(stdout, "5autsch\n");
		err_sys("pkt_send");
	}
	fprintf(stdout, "5\n");
	pkt_socket_close(&sck);
	fprintf(stdout, "6\n");
	pkt_free(&pkt);
	fprintf(stdout, "7\n");
	return 0;
}
