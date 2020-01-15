#include "../libpal.h"
#include "error.h"

#define SRC "192.168.1.2"
#define DST "127.0.0.1"

int main (void) {

	struct packet pkt;
	struct pkt_socket sck;
//	unsigned short int mss;
	
	fprintf(stdout, "1\n");
	pkt_init(&pkt, PKT_NET_IP, 60);
	fprintf(stdout, "2\n");
	pkt_ip_header(&pkt, 6, 4, 0, 56, 0, 0, 255, IPPROTO_ICMP, 0, 
			inet_addr(SRC), inet_addr(DST));
	fprintf(stdout, "3\n");
	pkt_move_actptr(&pkt, 20);
	pkt_ip_option_header(&pkt, PKT_IP_OPT_NOP, 1, 0, 0, NULL, 0);
	pkt_move_actptr(&pkt, 1);
	pkt_ip_option_header(&pkt, PKT_IP_OPT_NOP, 1, 0, 0, NULL, 0);
	pkt_move_actptr(&pkt, 1);
	pkt_ip_option_header(&pkt, PKT_IP_OPT_NOP, 1, 0, 0, NULL, 0);
	pkt_move_actptr(&pkt, 1);
	pkt_ip_option_header(&pkt, PKT_IP_OPT_END, 1, 0, 0, NULL, 0);
	pkt_move_actptr(&pkt, 1);
	pkt_icmp_header(&pkt, ICMP_REDIRECT, ICMP_REDIR_HOST, 0);
//	pkt_icmp_addr_mask(&pkt, 123, 1, 0, NULL);
//	pkt_icmp_dest_unreach(&pkt, 0);
	pkt_icmp_redirect(&pkt, 0, "123.234.123.234");
	pkt_move_actptr(&pkt, 8);
	/* build the ip header that caused the icmp unreachable error */
	pkt_ip_header(&pkt, 5, 4, 0, 257, 0, 0, 248, IPPROTO_UDP, 0xdead, 
			inet_addr(DST), inet_addr(SRC));
	pkt_move_actptr(&pkt, 20);
	pkt_add_data(&pkt, "get lost", 8);
	
	pkt_set_actptr(&pkt, 24);
	pkt_icmp_cksum(&pkt, 36);

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
