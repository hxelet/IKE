#ifndef __NX_PAYLOAD_H__
#define __NX_PAYLOAD_H__

#include "buffer.h"

/*
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Next Payload  |C|  RESERVED   |         Payload Length        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                            Nonce Data                         ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 */
typedef struct {
	buffer_t* data;
}nx_payload_t;

nx_payload_t*		nx_pld_create();
int					    nx_pld_pack(nx_payload_t* self, buffer_t* dst);
nx_payload_t*   nx_pld_unpack(int len, buffer_t* src);

#endif //__NX_PAYLOAD_H__
