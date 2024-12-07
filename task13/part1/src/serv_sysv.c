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

  msgid = msgget(key, IPC_CREAT | 0666);
  if (msgid == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  message.mtype = 1;
  strcpy(message.mtext, "Hi!");
  if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1) {
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }
  printf("Сервер отправил: %s\n", message.mtext);

  if (msgrcv(msgid, &message, sizeof(message.mtext), 2, 0) == -1) {
    perror("msgrcv");
    exit(EXIT_FAILURE);
  }

  printf("Полученное сообщение в сервере: %s\n", message.mtext);

  if (msgctl(msgid, IPC_RMID, NULL) == -1) {
    perror("msgctl");
    exit(EXIT_FAILURE);
  }

  return 0;
}