#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define PORT 8080

int main()
{
  int inet_socket;
  struct sockaddr_in server, client;
  char message[BUFF_SIZE];
  socklen_t client_len;
  int bytes;

  if ((inet_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  memset(&client, 0, sizeof(client));

  server.sin_family = AF_INET;
  if (inet_pton(AF_INET, "10.42.0.38", &server.sin_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }
  server.sin_port = htons(PORT);

  if (bind(inet_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  printf("Адрес и Номер порта Сервера: %s:%d\n",
         inet_ntoa(server.sin_addr),
         ntohs(server.sin_port));

  client_len = sizeof(client);
  while (1) {
    bytes = recvfrom(inet_socket,
                     message,
                     sizeof(message),
                     0,
                     (struct sockaddr*)&client,
                     &client_len);
    message[bytes] = '\0';
    printf("Сообщение от клиента: %s\n", message);

    if (sendto(inet_socket,
               "Hello from server",
               sizeof("Hello from server"),
               0,
               (struct sockaddr*)&client,
               client_len) < 0) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
    printf("Сервер отправил сообщение клиенту и завершает работу\n");
  }
  close(inet_socket);

  exit(EXIT_SUCCESS);
}