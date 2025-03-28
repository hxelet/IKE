#include "ke_payload.h"
#include "log.h"

static const char* module="KEPLD";

ke_payload_t* ke_pld_create() {
	ke_payload_t* self = calloc(1, sizeof(ke_payload_t));
	self->data = buf_create(0);

	return self;
}

int ke_pld_pack(ke_payload_t* self, buffer_t* dst) {
	buf_rwrite(dst, &self->dh_num, 2);
	buf_write(dst, NULL, 2);
	if(buf_copy(dst, self->data))
		return 0;

	return self->data->size + 4;
}

ke_payload_t* ke_pld_unpack(int len, buffer_t* src) {
	ke_payload_t* self = ke_pld_create();

	buf_rread(src, &self->dh_num, 2);
	logging(LL_DBG, module, "- dh number: %d", self->dh_num);
	buf_read(src, NULL, 2);
	buf_bread(src, self->data, len-4);
	logging(LL_DBG, module, "- key data (%d bytes)", len-4);
	logging_buf(LL_DBG, module, self->data);

	return self;
}
