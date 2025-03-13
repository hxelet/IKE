#include <stdio.h>
#include <string.h>
#include <libconfig.h>

#include "config.h"

configure_t* cnf_parse(const char* filename, int* count) {
	configure_t* self = NULL;
	config_t cf;
	config_setting_t *left, *right;
	const char* buf;

	if(!config_read_file(&cf, filename)) {
		printf("Failed read config file\n");
		config_destroy(&cf);
		return NULL;
	}

	config_setting_t* peers = config_lookup(&cf, "peers");
	if(peers != NULL) {
		*count = config_setting_length(peers);

		self = calloc(*count, sizeof(configure_t));
		for(int i = 0; i < *count; i++) {
			config_setting_t* cf_p = config_setting_get_elem(peers, i);

			left = config_setting_lookup(cf_p, "local");
			if(left != NULL) {
				if(config_setting_lookup_string(left, "ip", &buf))
					self[i].local.addr = net_stoa(buf);
			}

			right = config_setting_lookup(cf_p, "remote");
			if(right != NULL) {
				if(config_setting_lookup_string(right, "ip", &buf))
					self[i].remote.addr = net_stoa(buf);
			}

			if(config_setting_lookup_string(cf_p, "secret", &buf)) {
				self[i].secret = calloc(strlen(buf), sizeof(char));
				memcpy(self[i].secret, buf, strlen(buf));
			}

			printf("[CNF] Parsed peer %d\n", i+1);
		}
	}

	return self;
};
