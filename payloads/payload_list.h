#ifndef __PAYLOAD_LIST_H__
#define __PAYLOAD_LIST_H__

#include "payload.h"

typedef struct {
	payload_t* first;
	payload_t* last;
}payload_list_t;

payload_list_t*		plt_create();
void							plt_push(payload_list_t* self, payload_t* pld); // at last
payload_t*				plt_pop(payload_list_t* self);	// at first

int								plt_pack(payload_list_t* self, buffer_t* dst);
payload_list_t*		plt_unpack(payload_type type, buffer_t* src);

#endif //__PAYLOAD_LIST_H__
