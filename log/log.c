#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "daemon.h"

log_t* log_create() {
	log_t* self = calloc(1, sizeof(log_t));
	self->type = LT_STD;
	self->level = LL_ALL;

	return self;
}

void logging(LOG_LEVEL level, const char* module, const char* fmt, ...) {
	if(level < DMN.log->level)
		return;

	va_list args;
	va_start(args, fmt);
	printf("[%s] ", module);
	vprintf(fmt, args);
	printf("\n");

	va_end(args);
}
