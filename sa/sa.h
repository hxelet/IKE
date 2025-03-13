#ifndef __SA_H__
#define __SA_H__

#include "netype.h"

typedef struct {
	struct {
		ip4_addr addr;
	}local, remote;
}sa_t;

sa_t*		sa_create();

#endif //__SA_H__
