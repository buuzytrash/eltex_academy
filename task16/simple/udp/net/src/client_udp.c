#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE 32

int main(int argc, char** argv)
{
  int inet_socket;
  struct sockaddr_in server, client;
  char message[BUFF_SIZE];
  socklen_t server_len, client_len;
  int bytes;

  if (argc < 3) {
    perror("Формат ввода: ./client_udp адрес_сервера порт\n");
    exit(EXIT_FAILURE);
  }

  if ((inet_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  memset(&client, 0, sizeof(client));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(atoi(argv[2]));

  server_len = sizeof(server);

  if (sendto(inet_socket,
             "Hello from client",
             sizeof("Hello from client"),
             0,
             (struct sockaddr*)&server,
             server_len) < 0) {
    perror("send");
    exit(EXIT_FAILURE);
  }
  printf("Клиент отправил сообщение серверу \n");

  bytes = recvfrom(inet_socket,
                   message,
                   sizeof(message),
                   0,
                   (struct sockaddr*)&server,
                   (socklen_t*)&server_len);
  message[bytes] = '\0';
  printf("Сообщение от сервера: %s\n", message);

  close(inet_socket);

  exit(EXIT_SUCCESS);
}