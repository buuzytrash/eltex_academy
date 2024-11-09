#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFF_SIZE 32
#define SOCKET_PATH "/tmp/sv_sock_stream"

int main()
{
  unlink(SOCKET_PATH);

  int ssock, csock;
  struct sockaddr_un server, client;
  int server_len = sizeof(server);
  char message[BUFF_SIZE];
  int bytes;

  if ((ssock = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  memset(&client, 0, sizeof(client));

  server.sun_family = AF_LOCAL;
  strncpy(server.sun_path, SOCKET_PATH, sizeof(server.sun_path) - 1);

  if ((bind(ssock, (struct sockaddr*)&server, sizeof(server))) < 0) {
    perror("bind");
    close(ssock);
    exit(EXIT_FAILURE);
  }

  printf("TCP serverv is configured\n");

  if (listen(ssock, 3) < 0) {
    perror("listen");
    close(ssock);
    unlink(SOCKET_PATH);
    exit(EXIT_FAILURE);
  }

  if ((csock = accept(
           ssock, (struct sockaddr*)&server, (socklen_t*)&server_len)) < 0) {
    perror("accept");
    close(ssock);
    close(csock);
    unlink(SOCKET_PATH);
    exit(EXIT_FAILURE);
  }

  if ((bytes = read(csock, message, BUFF_SIZE)) < 0) {
    perror("read");
    close(ssock);
    close(csock);
    unlink(SOCKET_PATH);
    exit(EXIT_FAILURE);
  }

  message[bytes] = '\0';
  printf("Message from client %s\n", message);

  if ((send(csock,
            (const char*)"Hello from TCP server",
            strlen("Hello from TCP server"),
            0)) < 0) {
    perror("send");
    close(ssock);
    close(csock);
    unlink(SOCKET_PATH);
    exit(EXIT_FAILURE);
  }
  printf("Server sent message\n");

  unlink(SOCKET_PATH);
  close(ssock);
  close(csock);

  exit(EXIT_SUCCESS);
}