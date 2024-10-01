#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void handler(int signum)
{
  if (signum == SIGUSR1) {
    printf("Получен сигнал SIGUSR1\n");
  }
}

int main()
{
  struct sigaction sa;

  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  printf("Ожидание сигнала SIGUSR1...\n");
  while (1) {
    sleep(1);
  }

  return 0;
}
