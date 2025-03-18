#include <stdlib.h>

#include "sa.h"
#include "log.h"
#include "payload.h"

static const char* module="SA";

sa_t* sa_create() {
	sa_t* self = calloc(1, sizeof(sa_t));

	return self;
}

void sa_process(sa_t* self, buffer_t* buf) {
	payload_t* hdr = pld_unpack(buf, PT_HDR);
}

