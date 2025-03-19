#include "buffer.h"
#include <stdio.h>
#include <string.h>

buffer_t* buf_create(size_t capacity) {
	buffer_t* self = calloc(1, sizeof(buffer_t));
	self->capacity = capacity;
	self->data = calloc(capacity, sizeof(uint8_t));

	return self;
}

bool buf_free(buffer_t* self) {
	free(self->data);
	free(self);
	return true;
}

bool buf_write(buffer_t* self, void* data, size_t size, bool reverse) {
	// errors
	if(self == NULL) {
		printf("[BUF] buf is NULL\n");
		return false;
	}
	if(size == 0) {
		printf("[BUF] write size is wrong \n");
		return false;
	}

	size_t end_of_remains = (self->capacity - self->offset + self->size);
	if(size > end_of_remains) {
		size_t begin_of_remains = (self->capacity - self->size + 1);
		// realloc
		if(size > end_of_remains + begin_of_remains) {
			self->data = realloc(self->data, self->capacity+size);
			self->capacity += size;
		}
		// relocation
		memmove(self->data, self->data + self->offset, self->size);
		self->offset = 0;
	}

	uint8_t* dest = self->data + self->offset + self->size;
	if(data == NULL)
		memset(dest, 0, size);
	else {
		if(reverse) {
			uint8_t* s = data + size - 1;
			for(size_t i = 0; i < size; i++) {
				*dest = *s;
				dest++;
				s--;
			}
		}
		else
			memcpy(dest, data, size);
	}

	self->size += size;

	return true;
}

bool buf_read(buffer_t* self, void* dest, size_t size, bool reverse) {
	// errors
	if(self == NULL) {
		printf("[BUF] buf is NULL\n");
		return false;
	}
	if(size == 0 || size > self->size) {
		printf("[BUF] read size is wrong \n");
		return false;
	}

	if(dest != NULL) {
		uint8_t* d = dest;
		if(reverse) {
			uint8_t* s = self->data + self->offset + size - 1;
			for(size_t i = 0; i < size; i++) {
				*d = *s;
				d++;
				s--;
			}
		}
		else
			memcpy(dest, self->data + self->offset, size);
	}

	self->offset += size;
	self->size -= size;

	if(self->size == 0)
		self->offset = 0;

	return true;
}

bool buf_is_empty(buffer_t* self) {
	// errors
	if(self == NULL) {
		printf("[BUF] buf is NULL\n");
		return true;
	}
	return (self->size == 0);
}
