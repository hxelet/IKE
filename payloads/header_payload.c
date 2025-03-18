#include <stddef.h>

#include "header_payload.h"

static payload_rule_t rule[] = {
	{offsetof(header_payload_t, SPIi), 8, 0},
	{offsetof(header_payload_t, SPIr), 8, 0},
	{offsetof(header_payload_t, next_payload), 1, 0},
	{offsetof(header_payload_t, version), 1, 0},
	{offsetof(header_payload_t, exchange_type), 1, 0},
	{offsetof(header_payload_t, flags), 1, 0},
	{offsetof(header_payload_t, message_id), 4, 0},
	{offsetof(header_payload_t, length), 4, 0},
};

payload_t* hdr_pld_create() {
	payload_t* pld = calloc(1, sizeof(payload_t));
	pld->type = PT_HDR;
	pld->body = calloc(1, sizeof(header_payload_t));
	pld->rule = rule;
	pld->rule_count = 8;

	return pld;
}
