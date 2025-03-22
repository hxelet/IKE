#ifndef __SA_PAYLOAD_H__
#define __SA_PAYLOAD_H__

#include "buffer.h"

typedef struct {
}sa_payload_t;

int					    sa_pld_pack(sa_payload_t* self, buffer_t* dst);
sa_payload_t*   sa_pld_unpack(int len, buffer_t* src); 

#endif //__SA_PAYLOAD_H__
