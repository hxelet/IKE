#include <stdio.h>
#include <string.h>

#include "daemon.h"

daemon_t DMN;

void daemon_create() {
	memset(DMN.threads, 0, sizeof(DMN.threads));
	DMN.net = net_create();
	DMN.sam = sam_create();
}

void running() {
	for(int i =0; i < THREAD_MAX; i++) {
		if(DMN.threads[i].func != NULL) {
			if(pthread_create(&DMN.threads[i].id, NULL, DMN.threads[i].func, NULL) != 0) {
				printf("Failed pthread_create\n");
				return;
			}
			printf("Running job[%d]\n", i);
		}
	}

	for(int i =0; i < THREAD_MAX; i++) {
		if(DMN.threads[i].func != NULL) {
			pthread_join(DMN.threads[i].id, NULL);
			DMN.threads[i].func = NULL;
		}
	}

	printf("All threads finished\n");
}

bool push_job(void* (*func)(void*)) {
	for(int i =0; i < THREAD_MAX; i++) {
		if(DMN.threads[i].func == NULL) {
			DMN.threads[i].func = func;
			printf("Added a new job[%d]\n", i);
			return true;
		}
	}

	printf("Not enough thread space\n");
	return false;
}
