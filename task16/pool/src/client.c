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
  // server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &server.sin_addr.s_addr);

  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("connect");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Клиент подключился\n");

  bytes = recv(sock, message, BUFF_SIZE, 0);
  message[bytes] = '\0';
  close(sock);
  printf("Message: %s\n", message);

  printf("Получение данных...\n");
  char* message_server_ip = strtok(message, ":");
  char* message_server_port = strtok(NULL, ":");
  sock = socket(AF_INET, SOCK_STREAM, 0);
  printf("Данные получены!\n");
  printf("%s %s\n", message_server_ip, message_server_port);

  struct sockaddr_in message_server_addr;
  message_server_addr.sin_family = AF_INET;
  message_server_addr.sin_port = htons(atoi(message_server_port));
  inet_pton(AF_INET, message_server_ip, &message_server_addr.sin_addr);

  printf("Подключение для пересылки...\n");
  if (connect(sock,
              (struct sockaddr*)&message_server_addr,
              sizeof(message_server_addr)) < 0) {
    perror("connect");
    return 1;
  }
  memset(&message, '\0', sizeof(message));

  printf("Клиент начал пересылку сообщений\n");
  while (1) {
    printf("Введите сообщение: ");
    fgets(message, BUFF_SIZE, stdin);
    send(sock, message, strlen(message), 0);

    bytes = recv(sock, message, BUFF_SIZE, 0);
    message[bytes] = '\0';
    printf("Сообщение получено: %s\n", message);
  }

  printf("Клиент пересылку закончил\n");

  close(sock);

  exit(EXIT_SUCCESS);
}