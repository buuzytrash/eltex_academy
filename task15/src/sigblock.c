#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int signum)
{
  if (signum == SIGINT) {
    printf("Получен сигнал SIGINT\n");
  }
}

int main()
{
  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGINT);

  if (sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

  printf("SIGINT заблокирован, попробуйте ctrl+c\n");
  sleep(10);

  while (1) {
    sleep(1);
  }

  //   if (sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL) == -1) {
  //     perror("sigprocmask");
  //     exit(EXIT_FAILURE);
  //   }

  //   printf("SIGINT разблокирован, попробуйте ctrl+c\n");
  //   sleep(10);

  return 0;
}