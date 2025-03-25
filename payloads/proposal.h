#ifndef __PROPOSAL_H__
#define __PROPOSAL_H__

#include "buffer.h"
#include "transform.h"

/*
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Last Substruc |   RESERVED    |         Proposal Length       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Proposal Num  |  Protocol ID  |    SPI Size   |Num  Transforms|
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   ~                        SPI (variable)                         ~
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                        <Transforms>                           ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct proposal_t proposal_t;
struct proposal_t {
	int porotocol;
	buffer_t*			spi;
	transform_t*	transforms;

	proposal_t*		next;
};

#define pps_is_last(next_ptr) ((next_ptr) == NULL? 0 : 2)
proposal_t*		pps_create();
void					pps_push(proposal_t* self, proposal_t* newone);
int						pps_pack(proposal_t* self, buffer_t* dst);
proposal_t*		pps_unpack(buffer_t* src);

#endif //__PROPOSAL_H__
