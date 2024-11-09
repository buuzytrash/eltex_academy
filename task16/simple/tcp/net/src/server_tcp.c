#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE 32

int main()
{

  int ssock, csock;
  struct sockaddr_in server, client;
  int server_len = sizeof(server);
  char message[BUFF_SIZE];
  int bytes;

  if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  memset(&client, 0, sizeof(client));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = 0;

  if ((bind(ssock, (struct sockaddr*)&server, sizeof(server))) < 0) {
    perror("bind");
    close(ssock);
    exit(EXIT_FAILURE);
  }

  if (getsockname(ssock, (struct sockaddr*)&server, &server_len)) {
    perror("getsockname");
    exit(EXIT_FAILURE);
  }

  printf("TCP serverv is configured\n");
  printf("Адрес и Номер порта Сервера: %s %d\n",
         inet_ntoa(server.sin_addr),
         ntohs(server.sin_port));

  if (listen(ssock, 3) < 0) {
    perror("listen");
    close(ssock);
    exit(EXIT_FAILURE);
  }

  if ((csock = accept(
           ssock, (struct sockaddr*)&server, (socklen_t*)&server_len)) < 0) {
    perror("accept");
    close(ssock);
    close(csock);
    exit(EXIT_FAILURE);
  }

  if ((bytes = read(csock, message, BUFF_SIZE)) < 0) {
    perror("read");
    close(ssock);
    close(csock);
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
    exit(EXIT_FAILURE);
  }
  printf("Server sent message\n");

  close(ssock);
  close(csock);

  exit(EXIT_SUCCESS);
}