#include "tinydtls.h"
#include "lib/memb.h"
#include "dtls-support.h"
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include "clock-arch.h"
#include "timer.h"

#include "uip.h"
#include "pt.h"


static dtls_context_t the_dtls_context;
static dtls_cipher_context_t cipher_context;
static uint8_t lock_context = 0;

/* Log configuration */
#define LOG_MODULE "dtls-support"
#define LOG_LEVEL  LOG_LEVEL_DTLS
#include "dtls-log.h"


void
dtls_support_log_prefix(int level, const char *level_str, const char *module)
{
  struct timeval now;
  struct tm loctime;
  int n = 0;
  char buf[28];

  if((gettimeofday(&now, 0) == 0) &&
     (localtime_r(&now.tv_sec, &loctime) != NULL) &&
     (n = strftime(buf, sizeof(buf), "%F %T", &loctime)) > 0) {
    buf[n] = '\0';
    LOG_OUTPUT("%s.%03u [%s] %-4s - ", buf, (unsigned)(now.tv_usec / 1000),
               module, level_str);
  } else {
    LOG_OUTPUT("- [%s] %s - ", module, level_str);
  }
}

dtls_cipher_context_t *
dtls_cipher_context_acquire(void)
{
  return &cipher_context;
}

void
dtls_cipher_context_release(dtls_cipher_context_t *c)
{
  
}


void
memb_init(struct memb *m)
{
}


void *
memb_alloc(struct memb *m)
{
  return malloc(m->size);
}

char
memb_free(struct memb *m, void *ptr)
{
  free(ptr);
  return 1;
}


dtls_context_t *
dtls_context_acquire(void)
{
  if(lock_context) {
    return NULL;
  }
  lock_context = 1;
  return &the_dtls_context;
}

void
dtls_context_release(dtls_context_t *context)
{
  if(context == &the_dtls_context) {
    lock_context = 0;
  }
}

/* --------- time support ----------- */

void
dtls_ticks(dtls_tick_t *t)
{
  *t = clock_time();
}

int
dtls_fill_random(uint8_t *buf, size_t len)
{
  FILE *urandom = fopen("/dev/urandom", "r");

  if(!urandom) {
    dtls_emerg("cannot initialize random\n");
    return 0;
  }

  if(fread(buf, 1, len, urandom) != len) {
    dtls_emerg("cannot fill random\n");
    fclose(urandom);
    return 0;
  }

  fclose(urandom);

  return 1;
}

// static void
// dtls_retransmit_callback(void *ptr)
// {
//   dtls_context_t *context;
//   clock_time_t now;
//   clock_time_t next;

//   context = ptr;

//   now = clock_time();

//   /* Just one retransmission per timer scheduling */
//   dtls_check_retransmit(context, &next, 0);

//   if(next != 0) {
//     ctimer_set(&context->support.retransmit_timer,
//                next <= now ? 1 : next - now,
//                dtls_retransmit_callback, context);
//   }
// }
/*---------------------------------------------------------------------------*/

// void ctimer_set()

void
dtls_set_retransmit_timer(dtls_context_t *context, unsigned int time)
{
  // Implement this using callback timer
  // Called from dtls.c line 1529

  // Blocking implementation

  // dtls_tick_t next;
  // struct timer ret_timer;
  // printf("time: %d\n", time);
  // timer_set(&ret_timer, time);
  // while (1) {
  //   if (timer_expired(&ret_timer)) {
  //     dtls_tick_t now = clock_time();
  //     dtls_check_retransmit(context, &next, 1);
  //     printf("next: %lu, now: %lu, %lu\n", next, next-now, (unsigned long) (next-now)/DTLS_TICKS_PER_SECOND );
  
  //     if (next != 0) {
  //       timer_set(&ret_timer, next <= now ? 1 * DTLS_TICKS_PER_SECOND : next - now);
  //     } else {
  //       break;
  //     }
  //   }
  // }
}

/* Implementation of session functions */
void
dtls_session_init(session_t *sess) {
  memset(sess, 0, sizeof(session_t));
}

int
dtls_session_equals(const session_t *a, const session_t *b) {
  return a->port == b->port
    && uip_ipaddr_cmp(&((a)->addr),&(b->addr));
}

void *
dtls_session_get_address(const session_t *a)
{
  return (void *)a;
}

int
dtls_session_get_address_size(const session_t *a)
{
  return sizeof(session_t);
}

void
dtls_session_log(const session_t *addr)
{
  LOG_OUTPUT("[");
  LOG_OUTPUT("[%x-%x]:%d",
		  addr->addr[0],
		  addr->addr[1],
		  addr->port);
  LOG_OUTPUT("]");
}

void
dtls_session_print(const session_t *addr)
{
  LOG_OUTPUT("[");
  LOG_OUTPUT("[%x-%x]:%d",
		  addr->addr[0],
		  addr->addr[1],
		  addr->port);
  LOG_OUTPUT("]");
}

/* The init */
void
dtls_support_init(void)
{
}
