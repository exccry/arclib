pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "log.h"
#include "queue.h"

#define ARCL_QUEUE_SIZE 10
#define ARCL_MX_THREADS 4

typedef struct
{
  void (*func)(void *);
  void *arg;
} arcl_task_t;

typedef struct threadpool
{
  arcl_task_t task_queue[ARCL_QUEUE_SIZE];
  int queue_size;
  int head;
  int tail;
  int count;
  pthread_t threads[ARCL_MAX_THREADS];
  pthread_mutex_t mutex;
  pthread_cond_t cond_not_empty;
  pthread_cond_t cond_not_full;
  int shutdown;
} arcl_thpool_t;

int arcl_init(void);

void arcl_terminate(void);

void *arcl_thpool_worker(void *arg);

void arcl_thpool_destroy(arcl_thpool_t *pool);

arcl_thpool_t *arcl_thpool_create(const int nthreads);

int arcl_thpool_init(arcl_thpool_t *pool, const int nthreads);

int arcl_thpool_free(arcl_thpool_t **p);

int arcl_thpool_add(arcl_thpool_t *pool, void (*func)(void *), void *arg);
