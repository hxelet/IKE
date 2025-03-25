#include "transform.h"
#include "log.h"

static const char* module="TRF";

transform_t* trf_create() {
	transform_t* self = calloc(1, sizeof(transform_t));
	self->attribute = buf_create(0);

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
		logging(LL_DBG, module, "- last: %d", is_last);

		buf_read(src, NULL, 1);

		buf_rread(src, &len, 2);
		logging(LL_DBG, module, "- length: %d", len);

		buf_read(src, &cur->type, 1);
		logging(LL_DBG, module, "- type: %d", cur->type);

		buf_read(src, NULL, 1);

		buf_rread(src, &cur->id, 2);
		logging(LL_DBG, module, "- id: %d", cur->id);

		if(len > 8) {
			int format = 0;
			int type = 0;
			int alen = 0;

			buf_rread(src, &type, 2);
			format = (type >> 15) & 0x1;
			type = type & 0x7FFF;
			logging(LL_DBG, module, "- attribute format: %u", format);
			logging(LL_DBG, module, "- attribute type: %u", type);

			if(format == 0)
				buf_rread(src, &alen, 2);
			else
				alen = 2;
			logging(LL_DBG, module, "- attribute length: %u", alen);

			buf_bread(src, cur->attribute, alen);
			logging(LL_DBG, module, "- attribute value:");
			logging_buf(LL_DBG, module, cur->attribute);
		}

		if(self)
			trf_push(self, cur);
		else
			self = cur;

		logging(LL_DBG, module, "");
	}
/*
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |A|       Attribute Type        |    AF=0  Attribute Length     |
   |F|                             |    AF=1  Attribute Value      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                   AF=0  Attribute Value                       |
   |                   AF=1  Not Transmitted                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

	return self;
}
