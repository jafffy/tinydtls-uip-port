#ifndef DTLS_SUPPORT_CONF_H_
#define DTLS_SUPPORT_CONF_H_

typedef int uip_tcp_appstate_t; // XXX: placeholder because I don't know what it is
#include "uip.h"
#include <stdint.h>

typedef struct {
  uip_ipaddr_t addr;
  uint16_t port;
} session_t;

#define DTLS_TICKS_PER_SECOND 1000

typedef uint64_t dtls_tick_t;

#define HAVE_ASSERT_H 1

#define LOG_CONF_OUTPUT_PREFIX(level, level_str, module) \
  dtls_support_log_prefix(level, level_str, module)

void dtls_support_log_prefix(int level, const char *level_str, const char *module);

#endif /* DTLS_SUPPORT_CONF_H_ */
