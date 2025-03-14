#include <stdio.h>

#include "daemon.h"

int main() {
	printf("hi\n");

	daemon_create();
	running();

	return 0; 
}
