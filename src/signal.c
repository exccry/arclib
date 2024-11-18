#include "signal.h"

static arcl_signal_t __arcl_signal = {NULL, 0, 0, 0, PTHREAD_MUTEX_INITIALIZER};

int arcl_signal_create(void)
{
  struct sigaction sa;
  sa.sa_handler = sig_handler;
  sa.flags = NULL;
  sigemptyset(&sa.sa_mask);

  int signals[] = {SIGINT, SIGTERM, SIGSEGV, SIGABRT, SIGFPE};
  for (int i = 0; i < ARRS(signals); ++i)
  {
    if (sigaction(signals[i], &sa, NULL) != 0)
    {
      _log(LL_FATAL, "Failed to register signal handler for %d", signals[i]);
      return -1;
    }
  }

  return 0;
}

int arcl_signal_register(arcl_thpool_t *p)
{

}

int arcl_signal_unregister(arcl_thpool_t *p);
