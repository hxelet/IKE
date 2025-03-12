#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct linked_list_node_t linked_list_node_t;
struct linked_list_node_t {
	void*								data;
	linked_list_node_t* prev;
	linked_list_node_t* next;
};

linked_list_t* llt_create() {
	linked_list_t* self = calloc(1, sizeof(linked_list_t));
	self->length = 0;

	return self;
}

linked_list_node_t* llt_node_create(void* data) {
	linked_list_node_t* node = calloc(1, sizeof(linked_list_node_t));
	node->data = data;

	return node;
}

bool llt_insert(linked_list_t* self, void* data, int position) {
	// errors
	if(self == NULL) {
		printf("[LLT] llt is NULL\n");
		return false;
	}
	if(data == NULL) {
		printf("[LLT] empty data\n");
		return false;
	}
	if((self->length != 0 && (position < 0 || position > self->length))
			|| (self->length == 0 && position != 0 && position != 1)) {
		printf("[LLT] position value is wrong(%d)\n", position);
		return false;
	}

	linked_list_node_t* node = llt_node_create(data);
	linked_list_node_t* cur = NULL;

	// empty llt
	if(self->length == 0) {
		self->first = self->last = node;
	}
	// at last
	else if(position == 0) {
		cur = self->last;
		cur->next = node;
		node->prev = cur;
		self->last = node;
	}
	// at first
	else if(position == 1) {
		cur = self->first;
		node->next = cur;
		cur->prev = node;
		self->first = node;
	}
	// at position
	else {
		cur = self->first;
		for(int i = 2; i < position; i++) cur = cur->next;
		node->prev = cur;
		node->next = cur->next;
		cur->next->prev = node;
		cur->next = node;
	}

	self->length++;
	return true;
}

void* llt_delete(linked_list_t* self, int position) {
	// errors
	if(self == NULL) {
		printf("[LLT] llt is NULL\n");
		return NULL;
	}
	if(self->length == 0) {
		printf("[LLT] llt is empty\n");
		return NULL;
	}
	if(position < 0 || position > self->length) {
		printf("[LLT] position value is wrong(%d)\n", position);
		return NULL;
	}

	linked_list_node_t* cur = NULL;

	// only one
	if(self->length == 1) {
		cur = self->first;
		self->first = self->last = NULL;
	}
	// at last
	else if(position == 0) {
		cur = self->last;
		self->last = cur->prev;
	}
	// at first
	else if(position == 1) {
		cur = self->first;
		self->first = cur->next;
	}
	// at position
	else {
		cur = self->first;
		for(int i = 1; i < position; i++) cur = cur->next;
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
	}

	void* ret = cur->data;
	free(cur);
	self->length--;
	return ret;
}

bool llt_free(linked_list_t* self) {
	if(self == NULL) {
		printf("[LLT] llt is NULL\n");
		return false;
	}

	linked_list_node_t* cur = self->first;
	while(cur != NULL) {
		linked_list_node_t* tmp = cur;
		cur = cur->next;
		free(tmp->data);
		free(tmp);
	}

	free(self);
	return true;
}
