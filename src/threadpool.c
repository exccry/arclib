#include "threadpool.h"

static void _arcl_print_creation_success(arcl_thpool_t *pool)
{
  printf("Threadpool created successfully\n"
         "Memory Address: @[%p] -- @[%p]\n"
         "Occupying %ld Bytes\n",
         pool, (pool + sizeof(*pool)), sizeof(*pool));
}

static void __arcl_task_write(arcl_thpool_t *p, void (*func)(void *), void *arg)
{
  p->task_queue[p->tail].func = func;
  p->task_queue[p->tail].arg = arg;
  p->tail = (p->tail + 1) % p->queue_size;
  p->count++;
}

static void __arcl_task_read(arcl_thpool_t *p, arcl_task_t *t)
{
  t->func = p->task_queue[p->head].func;
  t->arg = p->task_queue[p->head].arg;
  p->head = (p->head + 1) % p->queue_size;
  p->count--;
}

static void __arcl_task_exec_free(arcl_task_t *t)
{
  (*(t->func))(t->arg);
  free(t->arg);
}

int arcl_thpool_add(arcl_thpool_t *pool, void (*func)(void *), void *arg)
{
  if (!pool)
  {
    fprintf(stderr, "Threadpool is undefined\n");
    return -1;
  }

  pthread_mutex_lock(&(pool->mutex));

  while (pool->count == pool->queue_size && !pool->shutdown)
    pthread_cond_wait(&(pool->cond_not_full), &(pool->mutex));

  if (pool->shutdown)
  {
    pthread_mutex_unlock(&pool->mutex);
    return -1;
  }
  
  __arcl_task_write(pool, func, arg);

  pthread_cond_signal(&(pool->cond_not_empty));
  pthread_mutex_unlock(&(pool->mutex));

  return 0;
}

// consume or execute a task from the queue
void *arcl_thpool_worker(void *arg)
{
  arcl_thpool_t *pool = (arcl_thpool_t *)arg;
  arcl_task_t task;

  while (1)
  {
    pthread_mutex_lock(&(pool->mutex));

    while (pool->count == 0 && !pool->shutdown)
      pthread_cond_wait(&(pool->cond_not_empty), &(pool->mutex));

    if (pool->shutdown)
    {
      pthread_mutex_unlock(&(pool->mutex));
      pthread_exit(NULL);
    }

    __arcl_task_read(pool, &task);

    pthread_cond_signal(&(pool->cond_not_full));
    pthread_mutex_unlock(&(pool->mutex));

    __arcl_task_exec_free(&task);
  }

  return NULL;
}

void arcl_thpool_destroy(arcl_thpool_t *pool)
{
  pthread_mutex_lock(&(pool->mutex));
  pool->shutdown = 1;

  // unblock the condition for all threads
  // and block all threads from appling a wait on that condition ?
  pthread_cond_broadcast(&(pool->cond_not_empty));
  pthread_mutex_unlock(&(pool->mutex));

  for (int i = 0; i < MAX_THREADS; ++i)
    pthread_join(pool->threads[i], NULL);

  // cleanup
  pthread_mutex_destroy(&(pool->mutex));
  pthread_cond_destroy(&(pool->cond_not_empty));
  pthread_cond_destroy(&(pool->cond_not_full));
}

int arcl_thpool_init(arcl_thpool_t *pool, const int nthreads)
{
  if (!pool)
  {
    fprintf(stderr, "Pool is undefined\n");
    return -1;
  }

  pool->queue_size = QUEUE_SIZE;
  pool->count = pool->head = pool->tail = 0;
  pool->shutdown = 0;
  // pool->nthreads = nthreads <= MAX_THREADS ? nthreads : MAX_THREADS;

  pthread_mutex_init(&(pool->mutex), NULL);
  pthread_cond_init(&(pool->cond_not_empty), NULL);
  pthread_cond_init(&(pool->cond_not_full), NULL);

  for (int i = 0; i < ARCL_MAX_THREADS; ++i)
  {
    if (pthread_create(&(pool->threads[i]), NULL, arcl_thpool_worker, (void *)pool) != 0)
    {
      fprintf(stderr, "Failed to create thread for threadpool\n");
      return -1;
    }
  }

  _arcl_print_creation_success(pool);
  return 0;
}

int arcl_init(void)
{
  // signal creation, atexit binding, etc.
}

void arcl_terminate(void)
{

}
