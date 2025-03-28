#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "payload_list.h"
#include "log.h"

static const char* module="PLT";

payload_list_t* plt_create() {
	payload_list_t* self = calloc(1, sizeof(payload_list_t));

	return self;
}

void plt_push(payload_list_t* self, payload_t* pld) {
	// errors
	if(self == NULL) {
		logging(LL_ERR, module, "payload list is NULL");
		return;
	}
	if(pld == NULL) {
		logging(LL_ERR, module, "payload is NULL");
		return;
	}

	if(self->first == NULL) {
		self->first = self->last = pld;
	}
	else {
		self->last->next = pld;
		self->last->next_type = pld->type;
		self->last = pld;
	}
}

payload_t* plt_pop(payload_list_t* self){
	// errors
	if(self == NULL) {
		logging(LL_ERR, module, "payload list is NULL");
		return NULL;
	}
	if(self->first == NULL) {
		logging(LL_ERR, module, "payload list is empty");
		return NULL;
	}

	payload_t* ret = self->first;
	self->first = ret->next;

	return ret;
}

buffer_t* plt_pack(payload_list_t* self) {
	buffer_t* dst = buf_create(0);
	size_t len = 0;

	for(payload_t* pld = self->first;
			pld != NULL;
			pld = pld->next) {
		len += pld_pack(pld, dst);
	}

	if(len != dst->size) {
		logging(LL_ERR, module, "wrong length");
		return NULL;
	}

	return dst;
}

payload_list_t* plt_unpack(payload_type type, buffer_t* src) {
	payload_list_t* self = plt_create();

	for(payload_type cur = type;
			cur != PT_NO; ) {
		payload_t* pld = pld_unpack(cur, src);
		if(pld == NULL)
			return NULL;
		cur = pld->next_type;
		plt_push(self, pld);
	}

	return self;
}

bool _plt_has(payload_list_t* self, ...) {
	int len = PT_EAP+1;
	uint8_t map[len];
	memset(map, 0, sizeof(map));
	va_list args;
	va_start(args, self);

	for(payload_t* cur = self->first; cur != NULL; cur = cur->next) {
		map[cur->type]++;
	}

	payload_type type = va_arg(args, payload_type);
	while(type != PT_NO) {
		if(map[type] == 0) {
			va_end(args);
			return false;
		}
		type = va_arg(args, payload_type);
	}
	va_end(args);

	return true;
}
