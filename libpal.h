/* $Id: libpal.h,v 1.1 2002/05/06 15:43:15 mkirchner Exp $

Copyright (C) 2002 Marc Kirchner <kirchner@stud.fh-heilbronn.de>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef _LIBPAL_H_
#define _LIBPAL_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "checksum.h"
#include "constants.h"

/*
* generic packet structure
*
* we maintain a so called "active ptr" inside the packet. Most operations
* on the packet will take place at the position the pointer points at.
* The pointer should never be moved directly, instead the functions
* pkt_set_actpr() and pkt_move_actptr() are provided.
* Using this approach we can perform bounds checking and make sure our
* library functions won't segfault.
* pkt_type is not really being used at the moment. I don't know if it
* ever will be. It might just be left out one day, so keep your fingers
* off. Use pkt_init() to set it. And dont even think of messing with the
* packet payload pointer pkt. You do NOT want to do that. Use the provided
* library functions.
* To be clear: don't do _anything_ with this struct. Just pass it as a
* parameter, be happy and your programs will (hopefully) work. 
*/
struct packet {
	unsigned long 	pkt_type;
	unsigned char	*pkt;
	unsigned int 	pkt_size;
	unsigned char	*pkt_ptr; /* active ptr inside packet */
	unsigned int	pkt_pos; /* pkt_ptr position, starting at 0 */
};

/*
* our socket structure
*
* same as above. Use the provided library fuctions to change its values.
* Do not do it on your own. Live and let die. You have been warned.
*/
struct pkt_socket {
	int rawfd;
	struct sockaddr_in *sckad;
	/*struct sockaddr_ll *sckll;*/
	socklen_t sckad_len;	
};

/* memory management */
int pkt_init(struct packet *pkt, unsigned long type, unsigned int size);
int pkt_free(struct packet *pkt);

/* pointer movement */
int pkt_set_actptr(struct packet *pkt, unsigned int bytepos);	
int pkt_move_actptr(struct packet *pkt, int relmov);

/* raw data */
int pkt_add_systimestamp(struct packet *pkt);
int pkt_add_data(struct packet *pkt, char *data, size_t data_len);
int pkt_resize(struct packet *pkt, unsigned int newsize);

/* socket operations */
int pkt_socket_open(struct pkt_socket *sck, int type);
int pkt_socket_close(struct pkt_socket *sck);
int pkt_socket_prepare(struct pkt_socket *sck, char *daddr);
int pkt_socket_setopt(struct pkt_socket *sck, int level, int optname, void *optval, socklen_t optlen);

/* sending */
int pkt_send(struct packet *pkt, struct pkt_socket *sck);

/* IP */
int pkt_ip_header(struct packet *pkt, unsigned int iphdr_len, unsigned int version, unsigned char tos, unsigned short int total_len, unsigned short int id, unsigned short int frag_off /* 3bit flag, 13bit offset */, unsigned char ttl, unsigned char protocol, unsigned short int cksum, unsigned int saddr, unsigned int daddr);
int pkt_ip_cksum(struct packet *pkt);
int pkt_ip_option_header(struct packet *pkt, unsigned char type, unsigned char len, unsigned char ptr, unsigned char oflw_flg, void *optval, size_t optlen);

/* TCP */
int pkt_tcp_header(struct packet *pkt, unsigned short int sport, unsigned short int dport, unsigned int seq, unsigned int ackseq, unsigned char headerlen, unsigned char reserved, unsigned char flags, unsigned short window, unsigned short int checksum, unsigned short int urgent);
int pkt_tcp_cksum(struct packet *pkt, char *saddr, char *daddr, unsigned int tcp_pkt_size);
int pkt_tcp_option(struct packet *pkt, unsigned char kind, unsigned char len, void *optval, size_t optlen);

/* UDP */
int pkt_udp_header(struct packet *pkt, unsigned short int sport, unsigned short int dport, unsigned short int udp_total_len, unsigned short int checksum);
int pkt_udp_cksum(struct packet *pkt, char *saddr, char *daddr, unsigned int udp_total_len);

/* ICMP */
int pkt_icmp_header(struct packet *pkt, unsigned char type, unsigned char code, unsigned short int checksum);
int pkt_icmp_cksum(struct packet *pkt, unsigned int len);
int pkt_icmp_addr_mask(struct packet *pkt, unsigned short int id, unsigned short int seqno, unsigned int mask, char *cmask);
int pkt_icmp_dest_unreach(struct packet *pkt, unsigned int unused);
int pkt_icmp_source_quench(struct packet *pkt, unsigned int unused);
int pkt_icmp_redirect(struct packet *pkt, unsigned int routerip, char *crouterip);
int pkt_icmp_echo(struct packet *pkt, unsigned short int id, unsigned short int seqno, void *data, size_t data_len);
int pkt_icmp_timestamp(struct packet *pkt, unsigned short int id, unsigned short int seqno, unsigned int ts_otime, unsigned int ts_rtime, unsigned int ts_ttime);

/* functions that might be useful and might be added some day ... 
int pkt_shift_data(struct packet *pkt, unsigned int from, unsigned int to, unsigned int len);
int pkt_tcp_change_seqno(int rel_seq, int rel_ackseq);
int pkt_tcp_set_seqno(unsigned int seq, unsigned int ackseq);
int pkt_ip_option(struct packet *pkt, unsigned char code, unsigned char len, unsigned char ptr);
int pkt_ip_option_addval(struct *pkt, unsigned char posptr, unsigned int optval);
*/

#endif
