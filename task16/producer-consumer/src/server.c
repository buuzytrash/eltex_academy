#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSG_SIZE 1024
#define BUFFER_SIZE 10
#define PORT 8080
#define MAX_CLIENTS 5
#define NUM_WORKERS 5

typedef struct
{
  int sock; // сокет для обслуживающего сервера
  short is_free; // флаг для проверки свободен ли сервер
} ServiceSrv;

typedef struct
{
  int sock; // сокет клиента
  int bytes; // количество байт полученных от клиента
  char message[MSG_SIZE]; // сообщение от клиента
} Item;

typedef struct
{
  Item items[MAX_CLIENTS];
  int head;
  int tail;
  int size;

  pthread_mutex_t mutex;
  pthread_cond_t cond;
} BufferQueue; // очередь для заявок

ServiceSrv serv_srvs[MAX_CLIENTS];
BufferQueue queue;

void init_queue(BufferQueue* q)
{
  q->head = 0;
  q->tail = 0;
  q->size = 0;

  pthread_mutex_init(&q->mutex, NULL);
  pthread_cond_init(&q->cond, NULL);
}

void push_back(BufferQueue* q, Item it)
{
  pthread_mutex_lock(&q->mutex);

  q->items[q->tail] = it;
  q->tail = (q->tail + 1) % MAX_CLIENTS;
  q->size++;
  pthread_cond_signal(&q->cond);

  pthread_mutex_unlock(&q->mutex);
}

Item pop_front(BufferQueue* q)
{
  pthread_mutex_lock(&q->mutex);

  while (q->size == 0) {
    pthread_cond_wait(&q->cond, &q->mutex);
  }

  Item it = q->items[q->head];
  q->head = (q->head + 1) % MAX_CLIENTS;
  q->size--;

  pthread_mutex_unlock(&q->mutex);

  return it;
}

void* consumer()
{
  while (1) {
    Item item = pop_front(&queue);
    send(item.sock, item.message, item.bytes, 0);
  }
  return NULL;
}

void* service_work(void* arg)
{
  ServiceSrv* srv = (ServiceSrv*)arg;
  char* message[MSG_SIZE];
  int n_bytes;

  while (1) {
    pthread_mutex_lock(&queue.mutex);
    while (srv->is_free) {
      pthread_mutex_unlock(&queue.mutex);
      usleep(10);
      pthread_mutex_lock(&queue.mutex);
    }
    pthread_mutex_unlock(&queue.mutex);

    memset(message, 0, MSG_SIZE);

    while ((n_bytes = recv(srv->sock, message, MSG_SIZE, 0)) > 0) {
      message[n_bytes] = '\0';
      printf("Сервер %d получил сообщение:%s\n", srv->sock, message);

      Item item;
      item.sock = srv->sock;
      strncpy(item.message, message, MSG_SIZE);
      item.bytes = n_bytes;
      push_back(&queue, item);
      memset(message, 0, MSG_SIZE);
    }

    if (n_bytes <= 0) {
      close(srv->sock);
      pthread_mutex_lock(&queue.mutex);
      srv->is_free = 1;
      pthread_mutex_unlock(&queue.mutex);
      continue;
    }
    pthread_mutex_lock(&queue.mutex);
    srv->is_free = 1;
    pthread_mutex_unlock(&queue.mutex);
  }
  return NULL;
}

int main()
{
  int ssock, csock;
  struct sockaddr_in srv_addr, clnt_addr;
  socklen_t srv_len = sizeof(srv_addr), clnt_len = sizeof(clnt_addr);

  if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  srv_addr.sin_port = htons(PORT);

  if ((bind(ssock, (struct sockaddr*)&srv_addr, srv_len)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(ssock, MAX_CLIENTS) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Сервер настроен и ожидает подключений: %s %d\n",
         inet_ntoa(srv_addr.sin_addr),
         ntohs(srv_addr.sin_port));

  init_queue(&queue);

  pthread_t pthread_service[NUM_WORKERS];
  for (int i = 0; i < NUM_WORKERS; ++i) {
    serv_srvs[i].is_free = 1;
    serv_srvs[i].sock = -1;

    if ((pthread_create(
            &pthread_service[i], NULL, service_work, &serv_srvs[i])) < 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  pthread_t pthread_consumer[NUM_WORKERS];
  for (int i = 0; i < NUM_WORKERS; ++i) {
    if ((pthread_create(&pthread_consumer[i], NULL, consumer, NULL)) < 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  while (1) {
    if ((csock = accept(ssock, (struct sockaddr*)&clnt_addr, &clnt_len)) < 0) {
      perror("accept");
      continue;
    }

    pthread_mutex_lock(&queue.mutex);
    for (int i = 0; i < NUM_WORKERS; ++i) {
      if (serv_srvs[i].is_free) {
        serv_srvs[i].sock = csock;
        serv_srvs[i].is_free = 0;
        break;
      }
    }
    pthread_mutex_unlock(&queue.mutex);
  }

  close(ssock);
  exit(EXIT_SUCCESS);
}