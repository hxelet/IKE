#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "network.h"

typedef struct {
	network_t* net;
}daemon_t;

extern daemon_t DMN;

void daemon_create();

#endif //__DAEMON_H__
