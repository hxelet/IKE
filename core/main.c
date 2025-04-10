#include <stdio.h>

#include "daemon.h"

int main() {
	printf("hi\n");

	daemon_create();

	sam_setup_by_config(DMN.sam, "/workspace/conf/local.conf");

	running();

	return 0; 
}
