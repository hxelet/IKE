#ifndef __TRANFORM_H__
#define __TRANFORM_H__

#include "buffer.h"

/*
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Last Substruc |   RESERVED    |        Transform Length       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Transform Type |   RESERVED    |          Transform ID         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                      Transform Attributes                     ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct transform_t transform_t;
struct transform_t {
	int type;
	int id;

	transform_t*	next;
};

#define trf_is_last(next_ptr) ((next_ptr) == NULL? 0 : 3)
transform_t*	trf_create();
void					trf_push(transform_t* self, transform_t* newone);
int						trf_pack(transform_t* self, buffer_t* dst);
transform_t*	trf_unpack(buffer_t* src);

#endif //__TRANFORM_H__
