#include <stdlib.h>

#include "sa_manager.h"
#include "sa.h"
#include "config.h"

sa_manager_t*	sam_create() {
	sa_manager_t* self = calloc(1, sizeof(sa_manager_t));
	self->sas.length = 0;
	self->sas.first = self->sas.last = NULL;

	return self;
}

bool sam_setup_by_config(sa_manager_t* self, const char* filename) {
	int len = 0;
	configure_t* cnf = cnf_parse(filename, &len);

	for(int i = 0; i < len; i++) {
		sa_t* sa = sa_create();
		sa->local.addr = cnf[i].local.addr;
		sa->remote.addr = cnf[i].remote.addr;
		llt_insert_at_last(&self->sas, sa);
	}

	return true;
}
