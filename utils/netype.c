#include <stdlib.h>

#include "netype.h"

packet_t* pkt_create(ip4_addr src, ip4_addr dst, buffer_t* data) {
	packet_t* self = calloc(1, sizeof(packet_t));
	self->src = src;
	self->dst = dst;
	self->data = data;

	return self;
}
