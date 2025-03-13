#ifndef __SA_MANAGER_H__
#define __SA_MANAGER_H__

#include "linked_list.h"

typedef struct {
	linked_list_t	sas;
}sa_manager_t;

sa_manager_t*		sam_create();
bool						sam_setup_by_config(sa_manager_t* self, const char* filename);

#endif //__SA_MANAGER_H__
