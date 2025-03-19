#include "payload.h"
#include "log.h"
#include "header_payload.h"

static const char* module="PLD";

bool pld_pack(payload_t* self, buffer_t* dst) {
	// errors
	if(self == NULL) {
		logging(LL_ERR, module, "payload is NULL");
		return false;
	}
	if(dst == NULL) {
		logging(LL_ERR, module, "buffer is NULL");
		return false;
	}
	if(self->body == NULL) {
		logging(LL_ERR, module, "payload is empty");
		return false;
	}
	if(self->rule == NULL) {
		logging(LL_ERR, module, "payload rule is NULL");
		return false;
	}
	if(self->rule_count == 0) {
		logging(LL_ERR, module, "not enough rule");
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

payload_t* pld_unpack_by_rule(buffer_t* src, payload_type type) {
	payload_t* pld;

	switch(type) {
		case PT_HDR:
			pld = hdr_pld_create();
			break;
		default:
			logging(LL_DBG, module, "Not yet supported payload [%s]", pld_type_string(type));
			return NULL;
	}

	logging(LL_DBG, module, "Unpacking %s ", pld_type_string(type));
	for(size_t i = 0; i < pld->rule_count; i++) {
		void* dst = pld->body + pld->rule[i].offset;
		size_t size = pld->rule[i].size;
		if(!buf_read(src, dst, size, pld->rule[i].reverse))
			return NULL;
		logging_hex(LL_DBG, module, dst, size);
	}
	logging(LL_DBG, module, "Finished %s Unpack", pld_type_string(type));

	return pld;
}

linked_list_t*	pld_unpack(buffer_t* src) {
	// errors
	if(src == NULL) {
		logging(LL_ERR, module, "buffer is NULL");
		return NULL;
	}

	linked_list_t* payloads = llt_create();
	payload_type next = PT_NO;

	// unpack header
	payload_t* pld = pld_unpack_by_rule(src, PT_HDR);
	next = ((header_payload_t*)pld->body)->next_payload;
	
	// unpack rest of
	for(payload_type type = next;
			!buf_is_empty(src) && type != PT_NO;
			type = next) {
		int length = 0;
		/* unpack general header
			 1                   2                   3
			 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
			 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			 | Next Payload  |C|  RESERVED   |         Payload Length        |
			 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

			 Figure 5: Generic Payload Header
		 */
		buf_read(src, &next, 1, 0);
		buf_read(src, NULL, 1, 0);	// Reserved
		buf_read(src, &length, 1, 0);

		// unbadk body
		payload_t* pld = pld_unpack_by_rule(src, type);
		if(pld == NULL)
			break;
		llt_insert_at_last(payloads, pld);
	}

	return payloads;
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
