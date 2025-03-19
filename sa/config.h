#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "netype.h"

typedef struct {
	struct {
		ip4_addr addr;
	}local, remote;
	char*		secret;

	uint64_t	SPIi;
	uint64_t	SPIr;
	uint8_t		version;
	uint32_t	message_id;
}configure_t;

configure_t* cnf_parse(const char* filename, int* count);

#endif //__CONFIG_H__
