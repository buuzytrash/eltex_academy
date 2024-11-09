#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 32

int main()
{
  int inet_socket;
  struct sockaddr_in server, client;
  char message[BUFF_SIZE];
  socklen_t server_len, client_len;
  int bytes;

  if ((inet_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  memset(&client, 0, sizeof(client));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = 0;

  if (bind(inet_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  server_len = sizeof(server);

  if (getsockname(inet_socket, (struct sockaddr*)&server, &server_len)) {
    perror("getsockname");
    exit(EXIT_FAILURE);
  }
  
  printf("Адрес и Номер порта Сервера: %s %d\n",
         inet_ntoa(server.sin_addr),
         ntohs(server.sin_port));

  client_len = sizeof(client);
  bytes = recvfrom(inet_socket,
                   message,
                   sizeof(message),
                   0,
                   (struct sockaddr*)&client,
                   (socklen_t*)&client_len);
  message[bytes] = '\0';
  printf("Сообщение от клиента: %s\n", message);

  if (sendto(inet_socket,
             "Hello from server",
             sizeof("Hello from server"),
             0,
             (struct sockaddr*)&client,
             client_len) < 0) {
    perror("send");
    exit(EXIT_FAILURE);
  }
  printf("Сервер отправил сообщение клиенту и завершает работу\n");

  exit(EXIT_SUCCESS);
}