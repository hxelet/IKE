#include "payload.h"
#include "log.h"
#include "sa_payload.h"
#include "nx_payload.h"
#include "ke_payload.h"
#include "n_payload.h"

static const char* module="PLD";

payload_t* pld_create(payload_type type) {
	payload_t* self = calloc(1, sizeof(payload_t));
	self->type = type;

	switch(type) {
		case PT_SA:
			self->body = calloc(1, sizeof(sa_payload_t));
			break;
		case PT_Nx:
			self->body = nx_pld_create();
			break;
		case PT_KE:
			self->body = ke_pld_create();
			break;
		case PT_N:
			self->body = n_pld_create();
			break;
		default: 
			break;
	}

	return self;
}

int pld_pack(payload_t* self, buffer_t* dst) {
	// errors
	if(self == NULL) {
		logging(LL_ERR, module, "payload is NULL");
		return false;
	}
	if(dst == NULL) {
		logging(LL_ERR, module, "dst is NULL");
		return false;
	}

	int len = 0;
	buffer_t* body = buf_create(0);

	switch(self->type) {
		case PT_SA:
			len = sa_pld_pack(self->body, body);
			break;
		case PT_Nx:
			len = nx_pld_pack(self->body, body);
			break;
		case PT_KE:
			len = ke_pld_pack(self->body, body);
			break;
		case PT_N:
			len = n_pld_pack(self->body, body);
			break;
		default: 
			return 0;
	}

	if(len > 0) {
		/* 
			 1                   2                   3
			 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
			 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
			 | Next Payload  |C|  RESERVED   |         Payload Length        |
			 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 */
		buf_write(dst, &self->next, 1);
		buf_write(dst, NULL, 1);
		buf_rwrite(dst, &len, 2);
		buf_merge(dst, body, true);
	}

	return len;
}

payload_t* pld_unpack(payload_type type, buffer_t* src) {
	// errors
	if(src == NULL) {
		logging(LL_ERR, module, "src is NULL");
		return false;
	}
	if(type == PT_NO) {
		logging(LL_ERR, module, "no type");
		return false;
	}

	payload_t* self = pld_create(type);
	int len = 0;

	/* 
		 1                   2                   3
		 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		 | Next Payload  |C|  RESERVED   |         Payload Length        |
		 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

		 Figure 5: Generic Payload Header
	 */
	buf_read(src, &self->next_type, 1);
	buf_read(src, NULL, 1);
	buf_rread(src, &len, 2);
	logging(LL_DBG, module, "%s payload", pld_type_string(type));

	len -= 4;
	switch(self->type) {
		case PT_SA:
			self->body = sa_pld_unpack(len, src);
			break;
		case PT_Nx:
			self->body = nx_pld_unpack(len, src);
			break;
		case PT_KE:
			self->body = ke_pld_unpack(len, src);
			break;
		case PT_N:
			self->body = n_pld_unpack(len, src);
			break;
		default: 
			return NULL;
	}

	return self;
}


char* pld_type_string(payload_type type) {
	switch(type) {
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
