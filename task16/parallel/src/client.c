#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFF_SIZE 32

int main(int argc, char** argv)
{
  int sock;
  struct sockaddr_in server;
  char message[BUFF_SIZE];
  int bytes;

  if (argc < 3) {
    perror("Формат ввода: ./client_udp адрес_сервера порт\n");
    exit(EXIT_FAILURE);
  }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    close(sock);
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));

  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&server, sizeof(server))) {
    perror("connect");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Клиент начал пересылку сообщений\n");
  for (int i = 0; i < 10; ++i) {
    memset(&message, 0, sizeof(message));
    sprintf(message, "Message %d from client\n", i);

    if (send(sock, message, strlen(message), 0) < 0) {
      perror("send");
      close(sock);
      exit(EXIT_FAILURE);
    }
    sleep(i);
  }
  printf("Клиент пересылку закончил\n");

  close(sock);

  exit(EXIT_SUCCESS);
}