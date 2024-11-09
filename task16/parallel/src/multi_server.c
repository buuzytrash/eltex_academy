#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFF_SIZE 1024

void reaper(int signal)
{
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (WIFEXITED(status)) {
      printf(
          "Сервер: процесс %d завершен с кодом %d\n", pid, WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
      printf(
          "Сервер: процесс %d завершен сигналом %d\n", pid, WTERMSIG(status));
    }
  }
}

int main()
{
  int ssock, csock;
  struct sockaddr_in server, client;
  socklen_t server_len, client_len;
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

  server_len = sizeof(server);

  if (bind(ssock, (struct sockaddr*)&server, server_len) < 0) {
    perror("bind");
    close(ssock);
    exit(EXIT_FAILURE);
  }

  if (getsockname(ssock, (struct sockaddr*)&server, &server_len) < 0) {
    perror("getsockname");
    close(ssock);
    exit(EXIT_FAILURE);
  }
  printf("TCP server is configured\n");
  printf("Адрес и Номер порта Сервера: %s %d\n",
         inet_ntoa(server.sin_addr),
         ntohs(server.sin_port));

  if (listen(ssock, 5) < 0) {
    perror("listen");
    close(ssock);
    exit(EXIT_FAILURE);
  }
  signal(SIGCHLD, reaper);

  for (;;) {
    client_len = sizeof(client);
    if ((csock = accept(ssock, (struct sockaddr*)&client, &client_len)) < 0) {
      perror("accept");
      close(ssock);
      close(csock);
      exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == 0) {
      close(ssock);

      while (1) {
        memset(message, 0, BUFF_SIZE);
        if ((bytes = recv(csock, message, BUFF_SIZE, 0)) <= 0) {
          if (bytes == 0) {
            printf("SERVER: Клиент закрыл соединение\n");
          }
          else {
            perror("recv");
          }
          break;
        }
        printf("SERVER: Socket для клиента - %d\n", csock);
        printf("SERVER: Длина сообщения - %d\n", bytes);
        printf("SERVER: Сообщение: %s\n\n", message);
      }

      close(csock);
      exit(EXIT_SUCCESS);
    }
    else if (pid > 0) {
      close(csock);
    }
    else {
      perror("fork");
      close(ssock);
      close(csock);
      exit(EXIT_FAILURE);
    }
  }
}