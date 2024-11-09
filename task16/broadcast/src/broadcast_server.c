#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8888
#define IP "255.255.255.255"
#define MESSAGE_SIZE 1024

int main()
{
  int sock;
  struct sockaddr_in broadcast;
  char* message = "Hello from broadcast server!";

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("sock");
    exit(EXIT_FAILURE);
  }

  memset(&broadcast, 0, sizeof(broadcast));
  broadcast.sin_family = AF_INET;
  broadcast.sin_port = htons(PORT);
  broadcast.sin_addr.s_addr = inet_addr(IP);

  int is_broadcast_enable = 1;
  if (setsockopt(sock,
                 SOL_SOCKET,
                 SO_BROADCAST,
                 &is_broadcast_enable,
                 sizeof(is_broadcast_enable)) < 0) {
    perror("setsockopt");
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (sendto(sock,
             message,
             sizeof(message),
             0,
             (struct sockaddr*)&broadcast,
             sizeof(broadcast)) < 0) {
    perror("sendto");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Сообщение разослано на порте %d\n", PORT);

  close(sock);
  return 0;
}