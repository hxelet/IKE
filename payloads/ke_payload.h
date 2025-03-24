#ifndef __KE_PAYLOAD_H__
#define __KE_PAYLOAD_H__

#include "buffer.h"

/*
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Next Payload  |C|  RESERVED   |         Payload Length        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   Diffie-Hellman Group Num    |           RESERVED            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                       Key Exchange Data                       ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct {
	int dh_num;
	buffer_t* data;
}ke_payload_t;

ke_payload_t*		ke_pld_create();
int					    ke_pld_pack(ke_payload_t* self, buffer_t* dst);
ke_payload_t*   ke_pld_unpack(int len, buffer_t* src);

#endif //__KE_PAYLOAD_H__
