#include <stdlib.h>
#include "network.h"

network_t* net_create() {
	network_t* self = calloc(1, sizeof(network_t));
	self->recv_que = que_create();
	self->send_que = que_create();

	return self;
}
