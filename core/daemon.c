#include <string.h>

#include "daemon.h"

daemon_t DMN;
static const char* module="DMN";

void daemon_create() {
	memset(DMN.threads, 0, sizeof(DMN.threads));
	DMN.log = log_create();
	DMN.net = net_create();
	DMN.sam = sam_create();
}

void running() {
	for(int i =0; i < THREAD_MAX; i++) {
		if(DMN.threads[i].func != NULL) {
			if(pthread_create(&DMN.threads[i].id, NULL, DMN.threads[i].func, NULL) != 0) {
				logging(LL_ERR, module, "Failed pthread_create");
				return;
			}
			logging(LL_DBG, module, "Running job[%d]", i);
		}
	}

	for(int i =0; i < THREAD_MAX; i++) {
		if(DMN.threads[i].func != NULL) {
			pthread_join(DMN.threads[i].id, NULL);
			DMN.threads[i].func = NULL;
		}
	}

	logging(LL_DBG, module, "All threads finished");
}

bool push_job(void* (*func)(void*)) {
	for(int i =0; i < THREAD_MAX; i++) {
		if(DMN.threads[i].func == NULL) {
			DMN.threads[i].func = func;
			logging(LL_DBG, module, "Added a new job[%d]", i);
			return true;
		}
	}

	logging(LL_ERR, module, "Not enough thread space");
	return false;
}
