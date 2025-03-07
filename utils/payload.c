#include "payload.h"
#include <stdio.h>

bool pld_pack(void* payload, payload_rule_t* rule, size_t rule_count, buffer_t* buf)
{
	// errors
	if(payload == NULL) {
		printf("[PLD] payload is NULL\n");
		return false;
	}
	if(buf == NULL) {
		printf("[PLD] buf is NULL\n");
		return false;
	}
	if(rule == NULL) {
		printf("[PLD] rule is NULL\n");
		return false;
	}
	if(rule_count == 0) {
		printf("[PLD] not enough rule\n");
		return false;
	}

	for(int i = 0; i < rule_count; i++) {
		void* src = payload+rule[i].offset;
		size_t size = rule[i].size;
		if(!buf_write(buf, src, size, rule[i].reverse))
			return false;
	}

	return true;
}

bool pld_unpack(void* payload, payload_rule_t* rule, size_t rule_count, buffer_t* buf)
{
	// errors
	if(payload == NULL) {
		printf("[PLD] payload is NULL\n");
		return false;
	}
	if(buf == NULL) {
		printf("[PLD] buf is NULL\n");
		return false;
	}
	if(rule == NULL) {
		printf("[PLD] rule is NULL\n");
		return false;
	}
	if(rule_count == 0) {
		printf("[PLD] not enough rule\n");
		return false;
	}

	for(int i = 0; i < rule_count; i++) {
		void* dest = payload+rule[i].offset;
		size_t size = rule[i].size;
		if(!buf_read(buf, dest, size, rule[i].reverse))
			return false;
	}

	return true;
}
