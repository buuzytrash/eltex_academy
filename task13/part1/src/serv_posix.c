#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 128
#define QUEUE_NAME "/my_queue"

int main()
{
  mqd_t mq;
  struct mq_attr attr;
  char message[N];
  char buffer[N];

  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = N;
  attr.mq_curmsgs = 0;

  mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
  if (mq == (mqd_t)-1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  strcpy(message, "Hi!");

  if (mq_send(mq, message, sizeof(message), 0) == -1) {
    perror("mq_send");
    exit(EXIT_FAILURE);
  }

  sleep(5);

  ssize_t read_bytes = mq_receive(mq, buffer, N, NULL);
  if (read_bytes == -1) {
    perror("mq_receive");
    exit(EXIT_FAILURE);
  }

  printf("Полученное сообщение от клиента: %s\n", buffer);

  mq_close(mq);
  mq_unlink(QUEUE_NAME);

  return 0;
}