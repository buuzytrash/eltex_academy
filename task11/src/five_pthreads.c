#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5

void* thread_call(void* args)
{
  int i = *(int*)args;
  printf("thread number %d\n", i);
  pthread_exit(NULL);
}

int main()
{
  pthread_t pthreads[N];
  int args[N];

  for (int i = 0; i < N; ++i) {
    args[i] = i;
    if (pthread_create(&pthreads[i], NULL, thread_call, (void*)&args[i]) != 0) {
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

  return 0;
}