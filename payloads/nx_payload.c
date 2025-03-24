#include "nx_payload.h"
#include "log.h"

static const char* module="NxPLD";

nx_payload_t* nx_pld_create() {
	nx_payload_t* self = calloc(1, sizeof(nx_payload_t));
	self->data = buf_create(0);

	return self;
}

int nx_pld_pack(nx_payload_t* self, buffer_t* dst) {
	if(buf_copy(dst, self->data))
		return 0;

	return self->data->size;
}

nx_payload_t* nx_pld_unpack(int len, buffer_t* src) {
	nx_payload_t* self = nx_pld_create();
	buf_bread(src, self->data, len);
	logging_buf(LL_DBG, module, self->data);

	return self;
}
