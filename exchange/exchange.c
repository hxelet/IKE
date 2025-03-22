#include <stdlib.h>

#include "exchange.h"
#include "log.h"

static const char* module="EXG";

exchange_t* exg_create() {
	exchange_t* self = calloc(1, sizeof(exchange_t));
	self->payloads = plt_create();

	return self;
}

buffer_t* exg_pack(exchange_t* self) {
	logging(LL_DBG, module, "Start Packing");

	buffer_t* body = buf_create(0);

	self->length = plt_pack(self->payloads, body);

	if(self->length == 0) {
		buf_free(body);
		return NULL;
	}

	self->length += 28;
	buffer_t* dst = buf_create(self->length);
	/*
													1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                       IKE SA Initiator's SPI                  |
		 |                                                               |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                       IKE SA Responder's SPI                  |
		 |                                                               |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |  Next Payload | MjVer | MnVer | Exchange Type |     Flags     |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                          Message ID                           |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                            Length                             |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 */
	buf_write(dst, &self->SPIi, 8);
	buf_write(dst, &self->SPIr, 8);
	buf_write(dst, &self->next_type, 1);
	buf_write(dst, &self->version, 1);
	buf_write(dst, &self->type, 1);
	buf_write(dst, &self->flags, 1);
	buf_rwrite(dst, &self->message_id, 4);
	buf_rwrite(dst, &self->length, 4);
	buf_merge(dst, body, true);

	logging(LL_DBG, module, "Finish Packing");

	return dst;
}

exchange_t* exg_unpack(buffer_t* buf) {
	exchange_t* self = exg_create();

	logging(LL_DBG, module, "Start Unpacking");
	/*
													1                   2                   3
			0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                       IKE SA Initiator's SPI                  |
		 |                                                               |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                       IKE SA Responder's SPI                  |
		 |                                                               |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |  Next Payload | MjVer | MnVer | Exchange Type |     Flags     |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                          Message ID                           |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 |                            Length                             |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 */
	buf_read(buf, &self->SPIi, 8);
	buf_read(buf, &self->SPIr, 8);
	buf_read(buf, &self->next_type, 1);
	buf_read(buf, &self->version, 1);
	buf_read(buf, &self->type, 1);
	buf_read(buf, &self->flags, 1);
	buf_rread(buf, &self->message_id, 4);
	buf_rread(buf, &self->length, 4);
	self->payloads = plt_unpack(self->next_type, buf);
	if(self->payloads == NULL) {
		return NULL;
	}

	logging(LL_DBG, module, "Finish Unpacking");
	return self;
}
