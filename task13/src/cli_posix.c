#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N 128
#define QUEUE_NAME "/my_queue"

int main()
{
  mqd_t mq;
  char message[N];
  char buffer[N];

  mq = mq_open(QUEUE_NAME, O_RDWR);

  strcpy(message, "Hello!");

  ssize_t read_bytes = mq_receive(mq, buffer, N, NULL);
  if (read_bytes == -1) {
    perror("mq_receive");
    exit(EXIT_FAILURE);
  }

  printf("Полученное сообщение от сервера: %s\n", buffer);

  if (mq_send(mq, message, sizeof(message), 0) == -1) {
    perror("mq_send");
    exit(EXIT_FAILURE);
  }

  mq_close(mq);

  return 0;
}