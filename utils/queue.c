#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

queue_t* que_create() {
	queue_t* self = calloc(1, sizeof(queue_t));
	self->body.length = 0;
	self->body.first = self->body.last = NULL;

	pthread_mutex_init(&self->mutex, NULL);
	pthread_cond_init(&self->cond, NULL);

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
	bool ret = false;
	// errors
	if(self == NULL) {
		printf("[QUE] queue is NULL\n");
		return false;
	}
	if(data == NULL) {
		printf("[QUE] empty data\n");
		return false;
	}

	pthread_mutex_lock(&self->mutex);

	ret = llt_insert_at_first(&self->body, data);

	pthread_cond_signal(&self->cond);
	pthread_mutex_unlock(&self->mutex);

	return ret;
}

void* que_deque(queue_t* self) {
	void* ret = NULL;
	// errors
	if(self == NULL) {
		printf("[QUE] queue is NULL\n");
		return NULL;
	}

	pthread_mutex_lock(&self->mutex);
	pthread_cond_wait(&self->cond, &self->mutex);

	ret = llt_delete_at_last(&self->body);

	pthread_mutex_unlock(&self->mutex);

	return ret;
}
