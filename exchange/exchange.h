#ifndef __EXCHANGE_H__
#define __EXCHANGE_H__

#include <stdint.h>

#include "payload_list.h"

typedef struct {
	uint64_t	SPIi;
	uint64_t	SPIr;
	uint8_t		next_type;
	uint8_t		version;
	uint8_t		type;
	uint8_t		flags;
	uint32_t	message_id;
	uint32_t	length;

	payload_list_t* payloads;
}exchange_t;

exchange_t*		exg_create();

buffer_t*			exg_pack(exchange_t* self);
exchange_t*		exg_unpack(buffer_t* buf);

#endif //__EXCHANGE_H__
