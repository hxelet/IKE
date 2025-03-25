#include "sa_payload.h"

sa_payload_t* sa_pld_create() {
	sa_payload_t* self = calloc(1, sizeof(sa_payload_t));

	return self;
}

int sa_pld_pack(sa_payload_t* self, buffer_t* dst) {
	return pps_pack(self->proposals, dst);
}

sa_payload_t* sa_pld_unpack(int len, buffer_t* src) {
	sa_payload_t* self = sa_pld_create();

	if(len > 0)
		self->proposals = pps_unpack(src);

	return self;
}
