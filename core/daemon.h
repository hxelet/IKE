#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <pthread.h>

#include "network.h"
#include "sa_manager.h"
#include "log.h"

#define THREAD_MAX 10

typedef struct {
	log_t*				log;
	network_t*		net;
	sa_manager_t* sam;

	struct {
		pthread_t id;
		void*			(*func)(void *);
	}threads[THREAD_MAX];
}daemon_t;

extern daemon_t DMN;

void daemon_create();
void running();
bool push_job(void* (*func)(void*));

#endif //__DAEMON_H__
