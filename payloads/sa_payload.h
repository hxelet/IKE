#ifndef __SA_PAYLOAD_H__
#define __SA_PAYLOAD_H__

#include "buffer.h"
#include "proposal.h"

/*
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Next Payload  |C|  RESERVED   |         Payload Length        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                          <Proposals>                          ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 */
typedef struct {
	proposal_t*			proposals;
}sa_payload_t;

sa_payload_t*		sa_pld_create();
int					    sa_pld_pack(sa_payload_t* self, buffer_t* dst);
sa_payload_t*   sa_pld_unpack(int len, buffer_t* src); 

#endif //__SA_PAYLOAD_H__
