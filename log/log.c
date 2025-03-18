#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "log.h"
#include "daemon.h"

log_t* log_create() {
  log_t* self = calloc(1, sizeof(log_t));
  self->type = LT_STD;
  self->level = LL_ALL;

  return self;
}

void logging(LOG_LEVEL level, const char* module, const char* fmt, ...) {
  if(level < DMN.log->level)
    return;

  va_list args;
  va_start(args, fmt);
  printf("[%s] ", module);
  vprintf(fmt, args);
  printf("\n");

  va_end(args);
}

void logging_buf(LOG_LEVEL level, const char* module, buffer_t* buf) {
  logging_hex(level, module, buf->data, buf->capacity);
}

void logging_hex(LOG_LEVEL level, const char* module, void* data, int size) {
  if(level < DMN.log->level)
    return;

  // width is fixed size; 16
  char hbuf[49] = {'\0',};  // hex buffer
  char cbuf[33] = {'\0',};  // char buffer

  // i = chunks's index
  // j = hex&char buf's index
  for(int i = 0, j = 0; i < size; i++, j++) {
    unsigned char c = 0xFF & ((char*)data)[i];

    // hex
    sprintf(&hbuf[j*3], "%02X ", c);

    // character
    if(isprint(c))
      sprintf(&cbuf[j*2], " %c", c);
    else
      sprintf(&cbuf[j*2], " .");

    // end of width OR chunk
    if(j == 15 || i+1 == size) {
      j++;
      hbuf[j*3] = cbuf[j*2] = '\0';
      j = -1;
      printf("[%s] %-48s %s\n", module, hbuf, cbuf);
    }
  }
}
