#include "n_payload.h"
#include "log.h"

static const char* module="NPLD";

n_payload_t* n_pld_create() {
	n_payload_t* self = calloc(1, sizeof(n_payload_t));
	self->spi = buf_create(0);
	self->data = buf_create(0);

	return self;
}

int n_pld_pack(n_payload_t* self, buffer_t* dst) {
	buf_write(dst, &self->protocol, 1);
	buf_write(dst, &self->spi->size, 1);
	buf_rwrite(dst, &self->type, 2);
	buf_copy(dst, self->spi);
	buf_copy(dst, self->data);

	return 4 + self->data->size + self->spi->size;
}

n_payload_t* n_pld_unpack(int len, buffer_t* src) {
	n_payload_t* self = n_pld_create();
	int spi_size = 0;

	buf_read(src, &self->protocol, 1);
	logging(LL_DBG, module, "- protocol: %d", self->protocol);

	buf_read(src, &spi_size, 1);
	logging(LL_DBG, module, "- spi size: %d", spi_size);

	buf_rread(src, &self->type, 2);
	logging(LL_DBG, module, "- type: %d(%s)", self->type, notify_type_string(self->type));

	buf_bread(src, self->spi, spi_size);
	logging(LL_DBG, module, "- spi (%d bytes)", self->spi->size);
	logging_buf(LL_DBG, module, self->spi);

	buf_bread(src, self->data, len - 4 - spi_size);
	logging(LL_DBG, module, "- data (%d bytes)", self->data->size);
	logging_buf(LL_DBG, module, self->data);


	return self;
}

char* notify_type_string(notify_type type) {
	switch (type) {
		case NT_NONE: return "NONE";
		case NT_UNSUPPORTED_CRITICAL_PAYLOAD: return "UNSUPPORTED CRITICAL PAYLOAD";
		case NT_INVALID_IKE_SPI: return "INVALID IKE SPI";
		case NT_INVALID_MAJOR_VERSION: return "INVALID MAJOR VERSION";
		case NT_INVALID_SYNTAX: return "INVALID SYNTAX";
		case NT_INVALID_MESSAGE_ID: return "INVALID MESSAGE ID";
		case NT_NO_PROPOSAL_CHOSEN: return "NO PROPOSAL CHOSEN";
		case NT_INVALID_KE_PAYLOAD: return "INVALID KE PAYLOAD";
		case NT_AUTHENTICATION_FAILED: return "AUTHENTICATION FAILED";
		case NT_SINGLE_PAIR_REQUIRED: return "SINGLE PAIR REQUIRED";
		case NT_NO_ADDITIONAL_SAS: return "NO ADDITIONAL SAS";
		case NT_INTERNAL_ADDRESS_FAILURE: return "INTERNAL ADDRESS FAILURE";
		case NT_FAILED_CP_REQUIRED: return "FAILED CP REQUIRED";
		case NT_TS_UNACCEPTABLE: return "TS UNACCEPTABLE";
		case NT_INVALID_SELECTORS: return "INVALID SELECTORS";
		case NT_UNACCEPTABLE_ADDRESSES: return "UNACCEPTABLE ADDRESSES";
		case NT_UNSUPPORTED_CRITICAL_OPTION: return "UNSUPPORTED CRITICAL OPTION";
		case NT_AUTHORIZATION_FAILED: return "AUTHORIZATION FAILED";
		case NT_CHILD_SA_NOT_FOUND: return "CHILD SA NOT FOUND";
		case NT_INITIAL_CONTACT: return "INITIAL CONTACT";
		case NT_SET_WINDOW_SIZE: return "SET WINDOW SIZE";
		case NT_ADDITIONAL_TS_POSSIBLE: return "ADDITIONAL TS POSSIBLE";
		case NT_IPCOMP_SUPPORTED: return "IPCOMP SUPPORTED";
		case NT_NAT_DETECTION_SOURCE_IP: return "NAT DETECTION SOURCE IP";
		case NT_NAT_DETECTION_DESTINATION_IP: return "NAT DETECTION DESTINATION IP";
		case NT_COOKIE: return "COOKIE";
		case NT_USE_TRANSPORT_MODE: return "USE TRANSPORT MODE";
		case NT_HTTP_CERT_LOOKUP_SUPPORTED: return "HTTP CERT LOOKUP SUPPORTED";
		case NT_REKEY_SA: return "REKEY SA";
		case NT_ESP_TFC_PADDING_NOT_SUPPORTED: return "ESP TFC PADDING NOT SUPPORTED";
		case NT_NON_FIRST_FRAGMENTS_ALSO: return "NON FIRST FRAGMENTS ALSO";
		case NT_MOBIKE_SUPPORTED: return "MOBIKE SUPPORTED";
		case NT_ADDITIONAL_IP4_ADDRESS: return "ADDITIONAL IP4 ADDRESS";
		case NT_ADDITIONAL_IP6_ADDRESS: return "ADDITIONAL IP6 ADDRESS";
		case NT_REDIRECT_SUPPORTED: return "REDIRECT SUPPORTED";
		case NT_REDIRECT: return "REDIRECT";
		case NT_ENDPOINT_DERIVATION_NEEDED: return "ENDPOINT DERIVATION NEEDED";
		case NT_USE_ASSIGNED_HoA: return "USE ASSIGNED HoA";
		case NT_PLATYPUS: return "PLATYPUS";
		case NT_ROHC_SUPPORTED: return "ROHC SUPPORTED";
		case NT_EAP_ONLY_AUTHENTICATION: return "EAP ONLY AUTHENTICATION";
		case NT_CHILDLESS_IKEV2_SUPPORTED: return "CHILDLESS IKEV2 SUPPORTED";
		case NT_QUICK_CRASH_DETECTION: return "QUICK CRASH DETECTION";
		case NT_IKEV2_MESSAGE_ID_SYNC_SUPPORTED: return "IKEV2 MESSAGE ID SYNC SUPPORTED";
		case NT_IPSEC_REPLAY_COUNTER_SYNC_SUPPORTED: return "IPSEC REPLAY COUNTER SYNC SUPPORTED";
		case NT_IKEV2_MESSAGE_ID_SYNC: return "IKEV2 MESSAGE ID SYNC";
		case NT_IPSEC_REPLAY_COUNTER_SYNC: return "IPSEC REPLAY COUNTER SYNC";
		case NT_SECURE_PASSWORD_METHODS: return "SECURE PASSWORD METHODS";
		case NT_PSK_PERSIST: return "PSK PERSIST";
		case NT_PSK_CONFIRM: return "PSK CONFIRM";
		case NT_ERX_SUPPORTED: return "ERX SUPPORTED";
		case NT_IFOM_CAPABILITY: return "IFOM CAPABILITY";
		case NT_SENDER_REQUEST_ID: return "SENDER REQUEST ID";
		case NT_IKEV2_FRAGMENTATION_SUPPORTED: return "IKEV2 FRAGMENTATION SUPPORTED";
		case NT_SIGNATURE_HASH_ALGORITHMS: return "SIGNATURE HASH ALGORITHMS";
		case NT_CLONE_IKE_SA_SUPPORTED: return "CLONE IKE SA SUPPORTED";
		case NT_CLONE_IKE_SA: return "CLONE IKE SA";
		case NT_PUZZLE: return "PUZZLE";
		case NT_USE_PPK: return "USE PPK";
		case NT_PPK_IDENTITY: return "PPK IDENTITY";
		case NT_NO_PPK_AUTH: return "NO PPK AUTH";
		case NT_INTERMEDIATE_EXCHANGE_SUPPORTED: return "INTERMEDIATE EXCHANGE SUPPORTED";
		case NT_IP4_ALLOWED: return "IP4 ALLOWED";
		case NT_IP6_ALLOWED: return "IP6 ALLOWED";
		case NT_ADDITIONAL_KEY_EXCHANGE: return "ADDITIONAL KEY EXCHANGE";
		case NT_USE_AGGFRAG: return "USE AGGFRAG";
		case NT_SUPPORTED_AUTH_METHODS: return "SUPPORTED AUTH METHODS";
		case NT_SA_RESOURCE_INFO: return "SA RESOURCE INFO";
		default: return "WTF!!";
	}
}
