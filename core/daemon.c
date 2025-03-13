#include "daemon.h"

daemon_t DMN;

void daemon_create() {
	DMN.net = net_create();
	DMN.sam = sam_create();
}
