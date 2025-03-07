#include "hexdump.h"
#include <stdio.h>
#include <ctype.h>

void hexdump(void* data, int size)
{
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
      printf("      %-48s %s\n", hbuf, cbuf);
    }
  }
}
