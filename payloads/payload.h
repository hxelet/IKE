#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include <stdlib.h>
#include <stdbool.h>
#include "buffer.h"
#include "linked_list.h"

typedef enum {
  PT_NO = 0,
	PT_HDR = 1,
  PT_SA = 33,
  PT_KE = 34,
  PT_IDi = 35,
  PT_IDr = 36,
  PT_CERT = 37,
  PT_CERTREQ = 38,
  PT_AUTH = 39,
  PT_Nx = 40,
  PT_N = 41,
  PT_D = 42,
  PT_V = 43,
  PT_TSi = 44,
  PT_TSr = 45,
  PT_SK = 46,
  PT_CP = 47,
  PT_EAP = 48
}payload_type;

typedef struct {
	size_t	offset;
	size_t	size;
	bool		reverse;
}payload_rule_t;

typedef struct {
	payload_type	type;
	void*					body;

  payload_rule_t*   rule;
  size_t            rule_count;
}payload_t;

bool						pld_pack(payload_t* self, buffer_t* dst);
linked_list_t*	pld_unpack(buffer_t* src);

char*						pld_type_string(payload_type type);

#endif //__PAYLOAD_H__
