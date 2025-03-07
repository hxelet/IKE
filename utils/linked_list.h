#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdbool.h>

typedef struct {
	int		length;
	void*	first;
	void* last;

}linked_list_t;

linked_list_t*	llt_create();

bool llt_insert(linked_list_t* self, void* data, int position);
#define llt_insert_at_first(self, data) llt_insert(self, data, 1)
#define llt_insert_at_last(self, data) llt_insert(self, data, 0)
#define llt_insert_at_position(self, data, position) llt_insert(self, data, position)

bool llt_delete(linked_list_t* self, void* dest, int position);
#define llt_delete_at_first(self, dest) llt_delete(self, dest, 1)
#define llt_delete_at_last(self, dest) llt_delete(self, dest, 0)
#define llt_delete_at_position(self, dest, position) llt_delete(self, dest, position)

bool llt_free(linked_list_t* self);

#endif //__LINKED_LIST_H__
