#include <stdio.h>

#include "payload.h"
#include "log.h"
#include "header_payload.h"

static const char* module="PLD";

bool pld_pack(payload_t* self, buffer_t* dst) {
	// errors
	if(self == NULL) {
		printf("[PLD] payload is NULL\n");
		return false;
	}
	if(dst == NULL) {
		printf("[PLD] buf is NULL\n");
		return false;
	}
	if(self->body == NULL) {
		printf("[PLD] body is NULL\n");
		return false;
	}
	if(self->rule == NULL) {
		printf("[PLD] rule is NULL\n");
		return false;
	}
	if(self->rule_count == 0) {
		printf("[PLD] not enough rule\n");
		return false;
	}

	for(size_t i = 0; i < self->rule_count; i++) {
		void* src = self->body + self->rule[i].offset;
		size_t size = self->rule[i].size;
		if(!buf_write(dst, src, size, self->rule[i].reverse))
			return false;
	}

	return true;
}

payload_t*	pld_unpack(buffer_t* src, payload_type type) {
	// errors
	if(src == NULL) {
		printf("[PLD] buf is NULL\n");
		return NULL;
	}
	if(type == PT_NO) {
		printf("[PLD] type is NULL\n");
		return NULL;
	}

	payload_t* self;

	switch(type) {
		case PT_HDR:
			self = hdr_pld_create();
			break;
		default:
			return NULL;
	}

	logging(LL_DBG, module, "Unpacking %s ", pld_type_string(type));
	for(size_t i = 0; i < self->rule_count; i++) {
		void* dst = self->body + self->rule[i].offset;
		size_t size = self->rule[i].size;
		if(!buf_read(src, dst, size, self->rule[i].reverse))
			return NULL;
		logging_hex(LL_DBG, module, dst, size);
	}
	logging(LL_DBG, module, "Finished %s Unpack", pld_type_string(type));

	return self;
}

char* pld_type_string(payload_type type) {
	switch (type){
		case PT_NO: return "No";
		case PT_HDR: return "IKE Header";
		case PT_SA: return "Security Association";
		case PT_KE: return "Key Exchange";
		case PT_IDi: return "Identification - Initiator";
		case PT_IDr: return "Identification - Responder";
		case PT_CERT: return "Certificate";
		case PT_CERTREQ: return "Certificate Request";
		case PT_AUTH: return "Authentication";
		case PT_Nx: return "Nonce";
		case PT_N: return "Notify";
		case PT_D: return "Delete";
		case PT_V: return "Vendor ID";
		case PT_TSi: return "Traffic Selector - Initiator";
		case PT_TSr: return "Traffic Selector - Responder";
		case PT_SK: return "Encrypted and Authenticated";
		case PT_CP: return "Configuration";
		case PT_EAP: return "Extensible Authentication";
		default: return "WTF!";
	}
}
