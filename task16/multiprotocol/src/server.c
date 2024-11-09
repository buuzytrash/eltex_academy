#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define MSG_SIZE 256
#define ADDR "127.0.0.1"
#define PORT 8080

void handle_tcp_client(int sock);
void handle_udp_client(int sock, struct sockaddr_in client, socklen_t len);

int main()
{
  int sock, udp_sock, tcp_sock;
  struct sockaddr_in server, client;
  socklen_t addr_len = sizeof(client);
  int max_fd;
  fd_set readfds;

  if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("tcp socket");
    exit(EXIT_FAILURE);
  }

  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("udp socket");
    close(tcp_sock);
    exit(EXIT_FAILURE);
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ADDR);
  server.sin_port = htons(PORT);

  if (bind(tcp_sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("tcp bind");
    close(tcp_sock);
    close(udp_sock);
    exit(EXIT_FAILURE);
  }
  if (bind(udp_sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("udp bind");
    close(tcp_sock);
    close(udp_sock);
    exit(EXIT_FAILURE);
  }

  if (listen(tcp_sock, 5) < 0) {
    perror("listen");
    close(tcp_sock);
    close(udp_sock);
    exit(EXIT_FAILURE);
  }

  printf("Сервер запущен на сервере %s:%d", ADDR, PORT);

  while (1) {
    FD_ZERO(&readfds);
    FD_SET(tcp_sock, &readfds);
    FD_SET(udp_sock, &readfds);

    max_fd = (tcp_sock > udp_sock) ? tcp_sock : udp_sock;

    if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
      perror("select");
      continue;
    }

    if (FD_ISSET(tcp_sock, &readfds)) {
      if ((sock = accept(tcp_sock, (struct sockaddr*)&client, &addr_len)) < 0) {
        perror("accept");
        continue;
      }
      handle_tcp_client(sock);
    }
    if (FD_ISSET(udp_sock, &readfds)) {
      handle_udp_client(udp_sock, client, addr_len);
    }
  }

  close(tcp_sock);
  close(udp_sock);
}

void handle_tcp_client(int sock)
{
  char message[MSG_SIZE];
  int n_bytes;
  memset(message, 0, MSG_SIZE);

  if ((n_bytes = recv(sock, message, MSG_SIZE - 1, 0)) < 0) {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  printf("Сообщения от TCP клиента: %s\n", message);

  const char response[] = "Ответ от сервера\0";
  send(sock, response, strlen(response), 0);
  close(sock);
}

void handle_udp_client(int sock, struct sockaddr_in client, socklen_t len)
{
  char message[MSG_SIZE];
  int n_bytes;
  memset(message, 0, MSG_SIZE);

  if ((n_bytes = recvfrom(
           sock, message, MSG_SIZE - 1, 0, (struct sockaddr*)&client, &len)) <
      0) {
    perror("recvfrom");
    exit(EXIT_FAILURE);
  }
  printf("Сообщения от UDP клиента: %s\n", message);

  const char response[] = "Ответ от сервера\0";
  sendto(sock, response, strlen(response), 0, (struct sockaddr*)&client, len);
}
