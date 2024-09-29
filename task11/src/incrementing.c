#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 18
#define ITERATIONS 10000000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long a = 0;

void* thread_calculate()
{
  for (int i = 0; i < ITERATIONS; ++i) {
    pthread_mutex_lock(&mutex);
    a += 1;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main()
{
  pthread_t pthreads[N];

  for (int i = 0; i < N; ++i) {
    if (pthread_create(&pthreads[i], NULL, thread_calculate, NULL) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < N; ++i) {
    if (pthread_join(pthreads[i], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }
  printf("a = %ld\n", a);

  pthread_mutex_destroy(&mutex);

  return 0;
}