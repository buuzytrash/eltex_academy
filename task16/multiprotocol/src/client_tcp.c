#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define MSG_SIZE 256
#define ADDR "127.0.0.1"
#define PORT 8080

int main()
{
  int sock;
  struct sockaddr_in server;
  char message[MSG_SIZE];
  int bytes;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    close(sock);
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  if (inet_pton(AF_INET, ADDR, &server.sin_addr) <= 0) {
    perror("address");
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr*)&server, sizeof(server))) {
    perror("connect");
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (send(sock,
           (const char*)"Hello from TCP client",
           strlen("Hello from TCP client"),
           0) < 0) {
    perror("send");
    close(sock);
    exit(EXIT_FAILURE);
  }
  printf("Client sent message\n");

  if ((bytes = recv(sock, message, MSG_SIZE, 0)) < 0) {
    perror("read");
    close(sock);
    exit(EXIT_FAILURE);
  }

  message[bytes] = '\0';
  printf("Message from server %s\n", message);

  close(sock);

  exit(EXIT_SUCCESS);
}