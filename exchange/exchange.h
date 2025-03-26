#ifndef __EXCHANGE_H__
#define __EXCHANGE_H__

#include <stdint.h>

#include "payload_list.h"

typedef enum {
  IKE_SA_INIT = 34,
  IKE_AUTH = 35,
  CREATE_CHILD_SA = 36,
  INFORMATIONAL = 37
}exchange_type;

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

char*					exg_type_string(exchange_type type);

#endif //__EXCHANGE_H__
