#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

queue_t* que_create() {
	queue_t* self = calloc(1, sizeof(queue_t));
	self->body.length = 0;
	self->body.first = self->body.last = NULL;

	return self;
}

bool que_free(queue_t* self) {
	if(self == NULL) {
		printf("[QUE] queue is NULL\n");
		return false;
	}
	if(!llt_free(&self->body))
		return false;

	free(self);
	return true;
}

bool que_enque(queue_t* self, void* data) {
	// errors
	if(self == NULL) {
		printf("[QUE] queue is NULL\n");
		return false;
	}
	if(data == NULL) {
		printf("[QUE] empty data\n");
		return false;
	}

	return llt_insert_at_first(&self->body, data);
}

void* que_deque(queue_t* self) {
	// errors
	if(self == NULL) {
		printf("[QUE] queue is NULL\n");
		return NULL;
	}

	return llt_delete_at_last(&self->body);
}
