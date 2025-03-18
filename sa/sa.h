#ifndef __SA_H__
#define __SA_H__

#include "netype.h"

typedef struct {
	struct {
		ip4_addr addr;
	}local, remote;
}sa_t;

sa_t*		sa_create();

void		sa_process(sa_t* self, buffer_t* buf);

#endif //__SA_H__
