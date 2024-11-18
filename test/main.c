#include "threadpool.h"

void sample_task(void *arg)
{
  int num = *((int *)arg);
  printf("Processing task: %d\n", num);
  sleep(1);
}

int main(void)
{
  arcl_thpool_t pool;
  arcl_thpool_init(&pool, ARCL_MAX_THREADS);

  for (int i = 0; i < 20; ++i)
  {
    int *arg = malloc(sizeof(*arg));
    *arg = i;
    arcl_thpool_add(&pool, sample_task, arg);
  }

  sleep(3);

  arcl_thpool_destroy(&pool);
  return 0;
}
