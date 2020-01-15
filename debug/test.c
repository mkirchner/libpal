#include "../packet.h"
#include "error.h"

#define SRC "192.168.1.2"
#define DST "127.0.0.1"

int main (void) {

	struct packet pkt;
	struct pkt_socket sck;
	unsigned short int mss;
	
	fprintf(stdout, "1\n");
	pkt_init(&pkt, PKT_NET_IP, 44);
	fprintf(stdout, "2\n");
	pkt_ip_header(&pkt, 5, 4, 0, 44, 0, 0, 255, IPPROTO_TCP, 0, 
			inet_addr(SRC), inet_addr(DST));
	fprintf(stdout, "3\n");
	pkt_move_actptr(&pkt, 20);
	pkt_tcp_header(&pkt, 22, 22, 123, 4, 5, 0, TH_ACK, 4096, 0, 0);
	//pkt_move_actptr(&pkt, 20);
	//mss = 0x00ff;
	//pkt_tcp_option(&pkt, PKT_TCP_OPT_MSS, PKT_TCP_OPT_MSS_LEN, &mss, sizeof(mss));
	//pkt_move_actptr(&pkt, -20);
	pkt_tcp_cksum(&pkt, SRC, DST, 24);
	if (pkt_socket_open(&sck, PKT_RAW) < 0) {
		fprintf(stdout, "4autsch\n");
		err_sys("&pkt_send");
	}
	pkt_socket_prepare(&sck, "127.0.0.1");
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
