#include "threadpool.h"
#include "log.h"

#define ARRS(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct
{
  arcl_thpool_t **allocated_pools;
  int allocated_count;
  volatile sig_atomic_t sig_received;
  pthread_mutex_t mutex;
} arcl_signal_t;


int arcl_signal_create(void (*sig_handler)(void));

int arcl_signal_register(arcl_thpool_t *p);

int arcl_signal_unregister(arcl_thpool_t *p);
