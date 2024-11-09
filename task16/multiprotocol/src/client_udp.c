#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG_SIZE 256
#define ADDR "127.0.0.1"
#define PORT 8080

int main()
{
  int sock;
  struct sockaddr_in server;
  char message[MSG_SIZE];
  socklen_t server_len;
  int bytes;

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &server.sin_addr) <= 0) {
    perror("address");
    exit(EXIT_FAILURE);
  }

  server_len = sizeof(server);

  if (sendto(sock,
             "Hello from client",
             sizeof("Hello from client"),
             0,
             (struct sockaddr*)&server,
             server_len) < 0) {
    perror("send");
    exit(EXIT_FAILURE);
  }
  printf("Клиент отправил сообщение серверу \n");

  bytes = recvfrom(sock,
                   message,
                   sizeof(message),
                   0,
                   (struct sockaddr*)&server,
                   (socklen_t*)&server_len);
  message[bytes] = '\0';
  printf("Сообщение от сервера: %s\n", message);

  close(sock);

  exit(EXIT_SUCCESS);
}