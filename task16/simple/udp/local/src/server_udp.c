
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// #define SOCKET_PATH
//   "/home/buuzytrash/eltex_academy/task16/simple/udp/local/my_socket"
#define SOCKET_PATH "./my_socket"

#define BUFF_SIZE 64

int main()
{
  unlink(SOCKET_PATH);

  int unix_socket;
  struct sockaddr_un server, client;
  char message[BUFF_SIZE];
  socklen_t socket_len;
  int n;

  if ((unix_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
    perror("socket create");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  memset(&client, 0, sizeof(client));

  server.sun_family = AF_UNIX;
  strncpy(server.sun_path, SOCKET_PATH, sizeof(server.sun_path) - 1);

  if (bind(unix_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("bind");
    close(unix_socket);
    exit(EXIT_FAILURE);
  }

  printf("UDP сервер настроен. Ждем подключения на сокете %s\n", SOCKET_PATH);
  socket_len = sizeof(client);

  n = recvfrom(unix_socket,
               (char*)message,
               BUFF_SIZE,
               MSG_WAITALL,
               (struct sockaddr*)&client,
               &socket_len);

  message[n] = '\0';
  printf("полученное сообщение от клиента: %s\n", message);

  if (sendto(unix_socket,
             (const char*)"Hello from UDP server",
             strlen("Hello from UDP server"),
             MSG_CONFIRM,
             (struct sockaddr*)&client,
             socket_len) < 0) {
    perror("sendto");
    unlink(SOCKET_PATH);
    exit(EXIT_FAILURE);
  }
  printf("Сервер отправил сообщение клиенту\n");

  unlink(SOCKET_PATH);
  // sleep(10);
  close(unix_socket);

  return 0;
}