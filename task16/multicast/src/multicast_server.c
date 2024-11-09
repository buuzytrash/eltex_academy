#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8888
#define IP "224.0.0.1"
#define MESSAGE_SIZE 1024

int main()
{
  int sock;
  struct sockaddr_in multicast;
  char* message = "Hello from multicast server!";

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("sock");
    exit(EXIT_FAILURE);
  }

  memset(&multicast, 0, sizeof(multicast));
  multicast.sin_family = AF_INET;
  multicast.sin_port = htons(PORT);
  multicast.sin_addr.s_addr = inet_addr(IP);

  if (sendto(sock,
             message,
             sizeof(message),
             0,
             (struct sockaddr*)&multicast,
             sizeof(multicast)) < 0) {
    perror("sendto");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Сообщение разослано на %s:%d\n", IP, PORT);

  close(sock);
  return 0;
}