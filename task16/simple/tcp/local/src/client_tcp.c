#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFF_SIZE 32
#define SV_SOCKET_PATH "/tmp/sv_sock_stream"

int main()
{
  int sock;
  struct sockaddr_un server;
  char message[BUFF_SIZE];
  int bytes;

  if ((sock = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    close(sock);
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));

  server.sun_family = AF_LOCAL;
  strncpy(server.sun_path, SV_SOCKET_PATH, sizeof(server.sun_path) - 1);

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

  if ((bytes = read(sock, message, BUFF_SIZE)) < 0) {
    perror("read");
    close(sock);
    exit(EXIT_FAILURE);
  }

  message[bytes] = '\0';
  printf("Message from server %s\n", message);

  close(sock);

  exit(EXIT_SUCCESS);
}