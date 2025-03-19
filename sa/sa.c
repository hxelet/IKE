#include <stdlib.h>

#include "sa.h"
#include "log.h"
#include "payload.h"
#include "header_payload.h"

static const char* module="SA";

sa_t* sa_create() {
	sa_t* self = calloc(1, sizeof(sa_t));

	return self;
}

void sa_process(sa_t* self, buffer_t* buf) {
	linked_list_t* plds = pld_unpack(buf);
	//header_payload_t* hdr = pld->body;
	//logging(LL_DBG, module, "next: %s", pld_type_string(hdr->next_payload));
}

