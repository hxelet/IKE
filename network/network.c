#include <stdlib.h>
#include <arpa/inet.h>

#include "network.h"
#include "daemon.h"

static const char* module="NET";

network_t* net_create() {
	network_t* self = calloc(1, sizeof(network_t));
	self->recv_que = que_create();
	self->send_que = que_create();

	self->port = 500;
	self->sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(self->sock < 0) {
		logging(LL_ERR, module, "Failed create socket");
		return NULL;
	}

	// for get IP_PKTINFO
	int opt = 1;
	setsockopt(self->sock, IPPROTO_IP, IP_PKTINFO, &opt, sizeof(opt));
	logging(LL_INFO, module, "Created socket");

	push_job(net_sending);
	push_job(net_receiving);

	return self;
}

bool net_send(ip4_addr src, ip4_addr dst, buffer_t* data) {
	packet_t* packet = pkt_create(src, dst, data);
	que_enque(DMN.net->send_que, packet);

	return true;
}

buffer_t* net_recv(ip4_addr* src, ip4_addr* dst) {
	packet_t* packet = que_deque(DMN.net->recv_que);
	if(src) *src = packet->src;
	if(dst) *dst = packet->dst;
	return packet->data;
}

void* net_receiving(void* arg) {
	(void)arg;
	struct sockaddr_in addr, client;
	int recv_len;
	char buf[1024];
	struct msghdr msg;
	struct iovec iov;
	char ancillary[64];
	struct cmsghdr *cm;
	struct in_pktinfo *pktinfo;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(DMN.net->port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(DMN.net->sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		logging(LL_ERR, module, "Failed bind socket");
		return NULL;
	}

	msg.msg_name = &client;
	msg.msg_namelen = sizeof(client);
	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = ancillary;
	msg.msg_controllen = sizeof(ancillary);
	msg.msg_flags = 0;

	while(1) {
		recv_len = recvmsg(DMN.net->sock, &msg, 0);
		if(recv_len) {
			for(cm = CMSG_FIRSTHDR(&msg); cm != NULL;
					cm = CMSG_NXTHDR(&msg, cm)) {
				if(cm->cmsg_level == IPPROTO_IP) {
					pktinfo = (struct in_pktinfo*)CMSG_DATA(cm);
					buffer_t* data = buf_create(recv_len);
					buf_write(data, buf, recv_len, false);

					packet_t* packet = pkt_create(
							client.sin_addr.s_addr,
							pktinfo->ipi_addr.s_addr,
							data
							);
					logging(LL_DBG, module, "Recived packet");
					que_enque(DMN.net->recv_que, packet);
					break;
				}
			}
		}
	}
}

void* net_sending(void* arg) {
	(void)arg;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = DMN.net->port;

	while(1) {
		packet_t* packet = que_deque(DMN.net->send_que);
		addr.sin_addr.s_addr = packet->dst;

		sendto(DMN.net->sock, packet->data->data+packet->data->offset, packet->data->size, 0, (struct sockaddr*)&addr, sizeof(addr));
		pkt_free(packet);
	}

}
