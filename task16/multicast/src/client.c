#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8888
#define IP "224.0.0.1"
#define BUFFER_SIZE 1024

int main()
{
  int sock;
  struct sockaddr_in server_addr, client_addr;
  struct ip_mreq mreq;
  char message[BUFFER_SIZE];
  socklen_t addr_len = sizeof(client_addr);

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind");
    close(sock);
    exit(EXIT_FAILURE);
  }

  mreq.imr_multiaddr.s_addr = inet_addr(IP);
  mreq.imr_interface.s_addr = INADDR_ANY;

  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) <
      0) {
    perror("setsockopt");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Клиент ждет сообщения\n");

  ssize_t num_bytes = recvfrom(sock,
                               message,
                               BUFFER_SIZE - 1,
                               0,
                               (struct sockaddr*)&client_addr,
                               &addr_len);
  if (num_bytes < 0) {
    perror("recvfrom");
    close(sock);
    exit(EXIT_FAILURE);
  }

  message[num_bytes] = '\0';

  printf("Сообщение: %s\n", message);
  close(sock);

  return 0;
}