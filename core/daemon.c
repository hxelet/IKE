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
	for(int i =0; i < THREAD_MAX; i++)
		pthread_join(DMN.threads[i].id, NULL);

	printf("All threads finished\n");
}

bool push_job(void* (*func)(void*)) {
	for(int i =0; i < THREAD_MAX; i++) {
		if(DMN.threads[i].is_running == 0) {
			int* arg = calloc(1, sizeof(int));
			*arg = i;
			if(pthread_create(&DMN.threads[i].id, NULL, func, arg) != 0) {
				printf("Failed pthread_create\n");
				free(arg);
				return false;
			}
			printf("Added a new job[%d]\n", i);
			return true;
		}
	}

	printf("Not enough thread space\n");
	return false;
}
