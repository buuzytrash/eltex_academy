#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define MAX_CLIENTS 5
#define PORT 8080

typedef struct
{
  int sock;
  short is_free;
  pthread_t pthread_id;
} serve_server;

serve_server serve_srvs[MAX_CLIENTS];
pthread_mutex_t mutex;

void* handle_client(void* arg)
{
  int csock = *(int*)arg;
  free(arg);

  char message[BUFF_SIZE];
  int bytes;

  while ((bytes = recv(csock, message, BUFF_SIZE, 0)) > 0) {
    message[bytes] = '\0';
    printf("Сообщение получено: %s\n", message);
    send(csock, message, bytes, 0);
  }

  close(csock);
  return NULL;
}

void* serve_server_handler(void* arg)
{
  serve_server* server = (serve_server*)arg;
  struct sockaddr_in client_addr;
  socklen_t cli_len = sizeof(client_addr);
  int csock;

  while (1) {
    if ((csock = accept(
             server->sock, (struct sockaddr*)&client_addr, &cli_len)) < 0) {
      perror("Ошибка accept ");
      continue;
    }

    int* csock_ptr = malloc(sizeof(int));
    *csock_ptr = csock;
    pthread_create(&server->pthread_id, NULL, handle_client, csock_ptr);
    pthread_detach(server->pthread_id);
  }

  close(server->sock);
  return NULL;
}

void* listener_handler(void* arg)
{
  int listener_sock = *(int*)arg;
  struct sockaddr_in client_addr;
  socklen_t cli_len = sizeof(client_addr);
  int csock;

  while (1) {
    if ((csock = accept(
             listener_sock, (struct sockaddr*)&client_addr, &cli_len)) < 0) {
      perror("Ошибка accept ");
      continue;
    }

    int serve_server_sock = -1;
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
      if (serve_srvs[i].is_free) {
        serve_srvs[i].is_free = 0;
        serve_server_sock = serve_srvs[i].sock;
        break;
      }
    }
    pthread_mutex_unlock(&mutex);

    if (serve_server_sock == -1) {
      close(csock);
      continue;
    }

    struct sockaddr_in serve_server_addr;
    socklen_t serve_server_len = sizeof(serve_server_addr);
    getsockname(serve_server_sock,
                (struct sockaddr*)&serve_server_addr,
                &serve_server_len);
    char serve_server_ip[INET_ADDRSTRLEN];
    inet_ntop(
        AF_INET, &serve_server_addr.sin_addr, serve_server_ip, INET_ADDRSTRLEN);
    int serve_server_port = ntohs(serve_server_addr.sin_port);

    char message[BUFF_SIZE];
    snprintf(
        message, sizeof(message), "%s:%d", serve_server_ip, serve_server_port);
    send(csock, message, strlen(message), 0);

    close(csock);
  }

  close(listener_sock);
  return NULL;
}

int main()
{
  int listener_sock;
  struct sockaddr_in listener_addr;

  for (int i = 0; i < MAX_CLIENTS; ++i) {
    if ((serve_srvs[i].sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("Ошибка socket ");
      printf("На итерации %d\n", i);

      exit(EXIT_FAILURE);
    }
    serve_srvs[i].is_free = 1;

    struct sockaddr_in serve_srvs_addr;
    serve_srvs_addr.sin_family = AF_INET;
    serve_srvs_addr.sin_addr.s_addr = INADDR_ANY;
    serve_srvs_addr.sin_port = htons(PORT + i + 1);

    if (bind(serve_srvs[i].sock,
             (struct sockaddr*)&serve_srvs_addr,
             sizeof(serve_srvs_addr)) < 0) {
      perror("Ошибка bind ");
      printf("На итерации %d\n", i);

      exit(EXIT_FAILURE);
    }

    if (listen(serve_srvs[i].sock, 1) < 0) {
      perror("Ошибка listen ");
      printf("На итерации %d\n", i);

      exit(EXIT_FAILURE);
    }

    pthread_create(
        &serve_srvs[i].pthread_id, NULL, serve_server_handler, &serve_srvs[i]);
  }

  listener_sock = socket(AF_INET, SOCK_STREAM, 0);
  listener_addr.sin_family = AF_INET;
  listener_addr.sin_addr.s_addr = INADDR_ANY;
  listener_addr.sin_port = htons(PORT);

  if (bind(listener_sock,
           (struct sockaddr*)&listener_addr,
           sizeof(listener_addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  printf("Сервер настроен. Адрес и порт: 127.0.0.1 8080\n");
  if (listen(listener_sock, 10) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  pthread_mutex_init(&mutex, NULL);
  pthread_t listener_pthread_id;
  int* listener_sock_ptr = malloc(sizeof(int));
  *listener_sock_ptr = listener_sock;
  pthread_create(
      &listener_pthread_id, NULL, listener_handler, listener_sock_ptr);
  pthread_detach(listener_pthread_id);

  for (int i = 0; i < MAX_CLIENTS; ++i) {
    pthread_join(serve_srvs[i].pthread_id, NULL);
  }

  pthread_mutex_destroy(&mutex);

  exit(EXIT_SUCCESS);
}