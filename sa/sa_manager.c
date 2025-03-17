#include <stdlib.h>

#include "sa_manager.h"
#include "sa.h"
#include "config.h"
#include "log.h"
#include "network.h"
#include "daemon.h"

static const char* module="SAM";

sa_manager_t*	sam_create() {
	sa_manager_t* self = calloc(1, sizeof(sa_manager_t));
	self->sas.length = 0;
	self->sas.first = self->sas.last = NULL;

	push_job(sam_running);

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
		logging(LL_INFO, module, "SA %d created", i+1);
	}

	return true;
}

void* sam_running(void* arg) {
	(void)arg;
	ip4_addr src, dst;
	buffer_t* buf;
	linked_list_t* sas = &DMN.sam->sas;

	while(1) {
		buf = net_recv(&src, &dst);
		if(buf != NULL) {
			llt_travel_reset(sas);
			for(sa_t* sa = llt_travel(sas);
					sa != NULL;
					sa = llt_travel(sas)) {
				if(dst == sa->local.addr && src == sa->remote.addr) {
					logging(LL_INFO, module, "matched!", buf->size);
					break;
				}
			}
		}
	}
}
