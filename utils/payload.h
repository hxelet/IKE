#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include <stdlib.h>
#include <stdbool.h>
#include "buffer.h"

typedef struct {
	size_t	offset;
	size_t	size;
	bool		reverse;
}payload_rule_t;

bool pld_pack(void* payload, payload_rule_t* rule, size_t rule_count, buffer_t* buf);
bool pld_unpack(void* payload, payload_rule_t* rule, size_t rule_count, buffer_t* buf);

#endif //__PAYLOAD_H__
