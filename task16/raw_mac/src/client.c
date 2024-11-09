#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
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
  uint8_t version_ihl;
  uint8_t tos;
  uint16_t total_len;
  uint16_t id;
  uint16_t flags_fragment_offset;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t sum;
  uint32_t src_addr;
  uint32_t dst_addr;
};

struct eth_header
{
  uint8_t dst_mac;
  uint8_t src_mac;
  uint16_t eth_type;
};

uint16_t calculate_checksum(uint16_t* addr, int len)
{
  int nleft = len;
  uint32_t sum = 0;
  uint16_t* w = addr;
  uint16_t answer = 0;

  while (nleft > 1) {
    sum += *w++;
    nleft -= 2;
  }

  if (nleft == 1) {
    *(uint8_t*)(&answer) = *(uint8_t*)w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  answer = ~sum;
  return answer;
}

int main()
{
  int sock;
  struct sockaddr_ll server;
  char message[BUFF_SIZE];
  socklen_t server_len;
  struct udp_header* udp_hdr;
  struct ip_header* ip_hdr;
  struct eth_header* eth_hdr;

  uint8_t dst_mac[ETH_ALEN] = {0x08, 0x97, 0x98, 0xaa, 0xd6, 0xe5};
  uint8_t src_mac[ETH_ALEN] = {0xe4, 0xaa, 0xea, 0xf2, 0xbc, 0x31};
  eth_hdr = (struct eth_hdr*)message;
  memcpy(eth_hdr->dst_mac, dst_mac, ETH_ALEN);
  memcpy(eth_hdr->src_mac, src_mac, ETH_ALEN);
  eth_hdr->eth_type = htons(ETH_P_IP);

  ip_hdr = (struct ip_header*)(message + sizeof(struct eth_header));
  ip_hdr->version_ihl = (4 << 4) | 5;
  ip_hdr->tos = 0;
  ip_hdr->total_len = htons(sizeof(struct ip_header) +
                            sizeof(struct udp_header) + strlen("hello") + 1);
  ip_hdr->flags_fragment_offset = 0;
  ip_hdr->ttl = 255;
  ip_hdr->protocol = IPPROTO_UDP;
  ip_hdr->sum = 0;
  ip_hdr->src_addr = inet_addr("10.42.0.38");
  ip_hdr->dst_addr = inet_addr("10.42.0.1");
  ip_hdr->sum = calculate_checksum((uint16_t*)ip_hdr, sizeof(struct ip_header));

  udp_hdr = (struct udp_header*)(message + sizeof(struct ip_header)) +
            sizeof(struct ip_header);
  udp_hdr->src_port = htons(HEADER_PORT);
  udp_hdr->dst_port = htons(PORT);
  udp_hdr->len = htons(sizeof(struct udp_header) + strlen("hello") + 1);
  udp_hdr->sum = 0;

  strcpy(message + sizeof(struct eth_header) + sizeof(struct ip_header) +
             sizeof(struct udp_header),
         "hello");

  if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  int optval = 1;
  if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) < 0) {
    perror("setsockopt");
    close(sock);
    exit(EXIT_FAILURE);
  }

  memset(&server, 0, sizeof(server));
  // server.sin_family = AF_INET;
  // server.sin_addr.s_addr = inet_addr(IP_NUM);
  // server.sin_port = htons(PORT);
  server.sll_family = AF_PACKET;
  server.sll_ifindex = if_nametoindex("wlp2s0");
  server.sll_halen = 6;
  memcpy(server.sll_addr, dst_mac, ETH_ALEN);

  server_len = sizeof(server);

  if (sendto(sock,
             message,
             sizeof(struct eth_header) + sizeof(struct ip_header) +
                 sizeof(struct udp_header) + strlen("hello") + 1,
             0,
             (struct sockaddr*)&server,
             server_len) < 0) {
    perror("sendto");
    close(sock);
    exit(EXIT_FAILURE);
  }
  printf("Клиент отправил сообщение серверу \n");
  close(sock);

  exit(EXIT_SUCCESS);
}