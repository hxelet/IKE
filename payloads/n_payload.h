#ifndef __N_PAYLOAD_H__
#define __N_PAYLOAD_H__

#include "buffer.h"

typedef enum {
  NT_NONE = 0,
  NT_UNSUPPORTED_CRITICAL_PAYLOAD = 1,
  NT_INVALID_IKE_SPI = 4,
  NT_INVALID_MAJOR_VERSION = 5,
  NT_INVALID_SYNTAX = 7,
  NT_INVALID_MESSAGE_ID = 9,
  NT_NO_PROPOSAL_CHOSEN = 14,
  NT_INVALID_KE_PAYLOAD = 17,
  NT_AUTHENTICATION_FAILED = 24,
  NT_SINGLE_PAIR_REQUIRED = 34,
  NT_NO_ADDITIONAL_SAS = 35,
  NT_INTERNAL_ADDRESS_FAILURE = 36,
  NT_FAILED_CP_REQUIRED = 37,
  NT_TS_UNACCEPTABLE = 38,
  NT_INVALID_SELECTORS = 39,
  NT_UNACCEPTABLE_ADDRESSES = 40,
  NT_UNSUPPORTED_CRITICAL_OPTION = 43,
  NT_AUTHORIZATION_FAILED = 45,
  NT_CHILD_SA_NOT_FOUND = 49,
  NT_INITIAL_CONTACT = 16384,
  NT_SET_WINDOW_SIZE = 16385,
  NT_ADDITIONAL_TS_POSSIBLE = 16386,
  NT_IPCOMP_SUPPORTED = 16387,
  NT_NAT_DETECTION_SOURCE_IP = 16388,
  NT_NAT_DETECTION_DESTINATION_IP = 16389,
  NT_COOKIE = 16390,
  NT_USE_TRANSPORT_MODE = 16391,
  NT_HTTP_CERT_LOOKUP_SUPPORTED = 16392,
  NT_REKEY_SA = 16393,
  NT_ESP_TFC_PADDING_NOT_SUPPORTED = 16394,
  NT_NON_FIRST_FRAGMENTS_ALSO = 16395,
  NT_MOBIKE_SUPPORTED = 16396,
  NT_ADDITIONAL_IP4_ADDRESS = 16404,
  NT_ADDITIONAL_IP6_ADDRESS = 16405,
  NT_REDIRECT_SUPPORTED = 16402,
  NT_REDIRECT = 16403,
  NT_ENDPOINT_DERIVATION_NEEDED = 16406,
  NT_USE_ASSIGNED_HoA = 16407,
  NT_PLATYPUS = 16415,
  NT_ROHC_SUPPORTED = 16416,
  NT_EAP_ONLY_AUTHENTICATION = 16417,
  NT_CHILDLESS_IKEV2_SUPPORTED = 16418,
  NT_QUICK_CRASH_DETECTION = 16419,
  NT_IKEV2_MESSAGE_ID_SYNC_SUPPORTED = 16420,
  NT_IPSEC_REPLAY_COUNTER_SYNC_SUPPORTED = 16421,
  NT_IKEV2_MESSAGE_ID_SYNC = 16422,
  NT_IPSEC_REPLAY_COUNTER_SYNC = 16423,
  NT_SECURE_PASSWORD_METHODS = 16424,
  NT_PSK_PERSIST = 16425,
  NT_PSK_CONFIRM = 16426,
  NT_ERX_SUPPORTED = 16427,
  NT_IFOM_CAPABILITY = 16428,
  NT_SENDER_REQUEST_ID = 16429,
  NT_IKEV2_FRAGMENTATION_SUPPORTED = 16430,
  NT_SIGNATURE_HASH_ALGORITHMS = 16431,
  NT_CLONE_IKE_SA_SUPPORTED = 16432,
  NT_CLONE_IKE_SA = 16433,
  NT_PUZZLE = 16434,
  NT_USE_PPK = 16435,
  NT_PPK_IDENTITY = 16436,
  NT_NO_PPK_AUTH = 16437,
  NT_INTERMEDIATE_EXCHANGE_SUPPORTED = 16438,
  NT_IP4_ALLOWED = 16439,
  NT_IP6_ALLOWED = 16440,
  NT_ADDITIONAL_KEY_EXCHANGE = 16441,
  NT_USE_AGGFRAG = 16442,
  NT_SUPPORTED_AUTH_METHODS = 16443,
  NT_SA_RESOURCE_INFO = 16444,
}notify_type;

/*
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Next Payload  |C|  RESERVED   |         Payload Length        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Protocol ID  |   SPI Size    |      Notify Message Type      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                Security Parameter Index (SPI)                 ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                                                               |
   ~                       Notification Data                       ~
   |                                                               |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct {
	int protocol;
	int type;
	buffer_t*		spi;
	buffer_t*		data;
}n_payload_t;

n_payload_t*		n_pld_create();
int					    n_pld_pack(n_payload_t* self, buffer_t* dst);
n_payload_t*		n_pld_unpack(int len, buffer_t* src);

char*           notify_type_string(notify_type type);

#endif //__N_PAYLOAD_H__
