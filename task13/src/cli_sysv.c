#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define N 128

typedef struct message_buffer
{
  long mtype;
  char mtext[N];
} msg_buf;

int main()
{
  key_t key;
  int msgid;

  msg_buf message;

  key = ftok(".", 'A');
  if (key == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  msgid = msgget(key, 0666);
  if (msgid == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) == -1) {
    perror("msgrcv");
    exit(EXIT_FAILURE);
  }

  printf("Полученное сообщение в клиенте: %s\n", message.mtext);

  message.mtype = 2;
  strcpy(message.mtext, "Hello!");
  if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1) {
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }
  return 0;
}