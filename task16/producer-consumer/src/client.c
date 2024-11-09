#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFF_SIZE 1024

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

  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &server.sin_addr.s_addr);

  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("connect");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Клиент подключился\n");
  printf("Клиент начал пересылку сообщений\n");
  while (1) {
    printf("Введите сообщение: ");
    fgets(message, BUFF_SIZE, stdin);
    message[strcspn(message, "\n")] = 0;
    if ((send(sock, message, BUFF_SIZE, 0)) < 0) {
      perror("send");
      close(sock);
      exit(EXIT_FAILURE);
    }
    int n_bytes = recv(sock, message, BUFF_SIZE, 0);
    if (n_bytes <= 0) {
      if (n_bytes == 0) {
        printf("Сервер недоступен\n");
      }
      else {
        perror("recv");
      }
      break;
    }
    message[n_bytes] = '\0';
    printf("Полученное сообщение: %s\n", message);
  }

  printf("Клиент пересылку закончил\n");

  close(sock);

  exit(EXIT_SUCCESS);
}