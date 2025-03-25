#include "transform.h"

transform_t* trf_create() {
	transform_t* self = calloc(1, sizeof(transform_t));

	return self;
}

void trf_push(transform_t* self, transform_t* newone) {
	if(self == NULL) {
		return;
	}

	transform_t* cur = self;
	for(; cur->next != NULL; cur = cur->next);
	cur->next =	newone;
}

int trf_pack(transform_t* self, buffer_t* dst) {
	int num_transform = 0;

	for(transform_t* cur = self; cur != NULL; cur = cur->next) {
		int is_last = trf_is_last(cur->next);
		int len = 8;

		buf_write(dst, &is_last, 1);
		buf_write(dst, NULL, 1);
		buf_rwrite(dst, &len, 2);
		buf_write(dst, &cur->type, 1);
		buf_write(dst, NULL, 1);
		buf_rwrite(dst, &cur->id, 2);
		num_transform++;
	}

	return num_transform;
}

transform_t* trf_unpack(buffer_t* src) {
	transform_t* self = NULL;
	int is_last = 3;

	while(is_last) {
		transform_t* cur = trf_create();
		int len = 0;

		buf_read(src, &is_last, 1);
		buf_read(src, NULL, 1);
		buf_rread(src, &len, 2);
		buf_read(src, &cur->type, 1);
		buf_read(src, NULL, 1);
		buf_rread(src, &cur->id, 2);
		if(len-8 > 0)
			buf_read(src, NULL, len-8);

		if(self)
			trf_push(self, cur);
		else
			self = cur;
	}

	return self;
}
