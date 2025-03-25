#include "proposal.h"
#include "log.h"

static const char* module="PPS";

proposal_t* pps_create() {
	proposal_t* self = calloc(1, sizeof(proposal_t));
	self->spi = buf_create(0);

	return self;
}

void pps_push(proposal_t* self, proposal_t* newone) {
	if(self == NULL) {
		return;
	}

	proposal_t* cur = self;
	for(; cur->next != NULL; cur = cur->next);
	cur->next =	newone;
}

int pps_pack(proposal_t* self, buffer_t* dst) {
	buffer_t* body = buf_create(0);
	int total_len = 0;
	int proposal_number = 0;

	for(proposal_t* cur = self; cur != NULL; cur = cur->next) {
		int num_transform = trf_pack(cur->transforms, body);
		int is_last = pps_is_last(cur->next);
		int len = 8 + body->size + cur->spi->size;

		buf_write(dst, &is_last, 1);
		buf_write(dst, NULL, 1);
		buf_rwrite(dst, &len, 2);
		proposal_number++;
		buf_write(dst, &proposal_number, 1);
		buf_write(dst, &cur->porotocol, 1);
		buf_write(dst, &cur->spi->size, 1);
		buf_write(dst, &num_transform, 1);
		buf_copy(dst, cur->spi);
		buf_merge(dst, body, false);
	}

	return total_len;
}

proposal_t* pps_unpack(buffer_t* src) {
	proposal_t* self = NULL;
	int is_last = 2;

	while(is_last) {
		proposal_t* cur = pps_create();
		int len = 0;
		int spi_size = 0;
		int num_transform = 0;
		int num_proposal = 0;

		buf_read(src, &is_last, 1);
		logging(LL_DBG, module, "- last: %d", is_last);

		buf_read(src, NULL, 1);

		buf_rread(src, &len, 2);
		logging(LL_DBG, module, "- length: %d", len);

		buf_read(src, &num_proposal, 1);
		logging(LL_DBG, module, "- num of proposal: %d", num_proposal);

		buf_read(src, &cur->porotocol, 1);
		logging(LL_DBG, module, "- protocol: %d", cur->porotocol);

		buf_read(src, &spi_size, 1);
		logging(LL_DBG, module, "- spi size: %d", spi_size);

		buf_read(src, &num_transform, 1);
		logging(LL_DBG, module, "- num of transform: %d", num_transform);

		buf_bread(src, cur->spi, spi_size);
		logging(LL_DBG, module, "- spi (%d bytes)", cur->spi->size);
		logging_buf(LL_DBG, module, cur->spi);

		logging(LL_DBG, module, "");

		if(num_transform > 0)
			cur->transforms = trf_unpack(src);

		if(self)
			pps_push(self, cur);
		else
			self = cur;
	}

	return self;
}
