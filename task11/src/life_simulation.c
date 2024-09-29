#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CUSTOMERS 3
#define SHOPS 5

int shops[SHOPS];
pthread_mutex_t shops_mutex[SHOPS];

int customers_needs[CUSTOMERS];
int custmers_satisfied = 0;
pthread_mutex_t satisfied_mutex = PTHREAD_MUTEX_INITIALIZER;

void* customers_life(void* arg)
{
  int customer_id = *(int*)arg;
  int need = customers_needs[customer_id];

  printf("Customer #%d with %d\n", customer_id, need);

  while (need > 0) {
    for (int i = 0; i < SHOPS; ++i) {
      pthread_mutex_lock(&shops_mutex[i]);
      if (shops[i] > 0) {
        int bought = (need < shops[i]) ? need : shops[i];
        shops[i] -= bought;
        need -= bought;

        printf("Customer %d: Shop %d, Bought %d, Need %d\n",
               customer_id,
               i,
               bought,
               need);
        pthread_mutex_unlock(&shops_mutex[i]);
        break;
      }
      pthread_mutex_unlock(&shops_mutex[i]);
    }
    sleep(2);
  }

  pthread_mutex_lock(&satisfied_mutex);
  custmers_satisfied++;
  pthread_mutex_unlock(&satisfied_mutex);

  pthread_exit(NULL);
}

void* loader_function(void* arg)
{
  while (custmers_satisfied < CUSTOMERS) {
    for (int i = 0; i < SHOPS; ++i) {
      pthread_mutex_lock(&shops_mutex[i]);
      int was = shops[i];
      shops[i] += 5000;
      int now = shops[i];
      printf("Loader: Shop %d, Was %d, Now %d\n", i, was, now);
      pthread_mutex_unlock(&shops_mutex[i]);
    }
    sleep(1);
  }
  pthread_exit(NULL);
}

int main()
{
  srand(time(NULL));

  pthread_t customers[CUSTOMERS];
  int customer_id[CUSTOMERS];

  for (int i = 0; i < SHOPS; ++i) {
    shops[i] = rand() % 10000 + 1;
    printf("shop #%d with products %d\n", i, shops[i]);
    pthread_mutex_init(&shops_mutex[i], NULL);
  }

  for (int i = 0; i < CUSTOMERS; ++i) {
    customers_needs[i] = rand() % 100000 + 1;
    customer_id[i] = i;
    if (pthread_create(&customers[i], NULL, customers_life, &customer_id[i]) !=
        0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  pthread_t loader;
  pthread_create(&loader, NULL, loader_function, NULL);

  for (int i = 0; i < CUSTOMERS; ++i) {
    if (pthread_join(customers[i], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }

  if (pthread_join(loader, NULL) != 0) {
    perror("pthread_join");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < SHOPS; ++i) {
    pthread_mutex_destroy(&shops_mutex[i]);
  }

  return 0;
}