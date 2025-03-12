#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "queue.h"

typedef struct {
	queue_t* send_que;
	queue_t* recv_que;
}network_t;

network_t* net_create();

#endif //__NETWORK_H__
