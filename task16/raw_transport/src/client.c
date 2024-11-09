#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define IP_NUM "127.0.0.1"
#define PORT 8080
#define HEADER_PORT 6969

struct udp_header
{
  uint16_t src_port;
  uint16_t dst_port;
  uint16_t len;
  uint16_t sum;
};

struct ip_header
{
  
}

int main()
{
  int sock;
  struct sockaddr_in server;
  char message[BUFF_SIZE];
  socklen_t server_len;
  struct udp_header* udp_hdr;

  if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(IP_NUM);
  server.sin_port = htons(PORT);

  server_len = sizeof(server);

  udp_hdr = (struct udp_header*)message;
  udp_hdr->src_port = htons(HEADER_PORT);
  udp_hdr->dst_port = htons(PORT);
  udp_hdr->len = htons(sizeof(struct udp_header) + strlen("hello") + 1);
  udp_hdr->sum = 0;

  strcpy(message + sizeof(struct udp_header), "hello");

  if (sendto(sock,
             message,
             sizeof(struct udp_header) + strlen("hello"),
             0,
             (struct sockaddr*)&server,
             server_len) < 0) {
    perror("sendto");
    close(sock);
    exit(EXIT_FAILURE);
  }
  printf("Клиент отправил сообщение серверу \n");
  char reciev[BUFF_SIZE];

  while (1) {
    int recv_len;
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);

    if ((recv_len = recvfrom(sock,
                             reciev,
                             BUFF_SIZE,
                             0,
                             (struct sockaddr*)&from_addr,
                             &addr_len)) < 0) {
      perror("recvfrom");
      close(sock);
      exit(EXIT_FAILURE);
    }

    if ((from_addr.sin_addr.s_addr == inet_addr(IP_NUM)) &&
        (from_addr.sin_port == htons(0))) {
      printf("Получено %d байт от %s:%d\n",
             recv_len,
             inet_ntoa(from_addr.sin_addr),
             ntohs(from_addr.sin_port));
      printf("Сообщение: %s\n", reciev + 28);
      close(sock);
      return 0;
    }
  }

  close(sock);

  exit(EXIT_SUCCESS);
}