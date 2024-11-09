
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "./my_socket"
#define BUFF_SIZE 64

int main()
{
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

  client.sun_family = AF_UNIX;
  strncpy(client.sun_path, "/tmp/client_socket", sizeof(client.sun_path) - 1);
  unlink(client.sun_path); // Удаляем сокет, если он уже существует

  if (bind(unix_socket, (struct sockaddr*)&client, sizeof(client)) < 0) {
    perror("bind");
    close(unix_socket);
    exit(EXIT_FAILURE);
  }

  socket_len = sizeof(server);
  if (sendto(unix_socket,
             (const char*)"Hello from UDP client",
             strlen("Hello from UDP client"),
             MSG_CONFIRM,
             (struct sockaddr*)&server,
             socket_len) < 0) {
    perror("sendto");
    exit(EXIT_FAILURE);
  }
  printf("Клиент отправил сообщение серверу\n");

  printf("начинаю получать сообщение от сервера\n");
  n = recvfrom(unix_socket,
               (char*)message,
               BUFF_SIZE,
               MSG_WAITALL,
               (struct sockaddr*)&server,
               &socket_len);
  printf("сообщение от сервера получил\n");
  message[n] = '\0';
  printf("Полученное сообщение от сервера %s\n", message);
  close(unix_socket);

  unlink(client.sun_path);

  // unlink(SOCKET_PATH);

  return 0;
}