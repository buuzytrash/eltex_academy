#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  sigset_t set;
  int signum;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);

  if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

  printf("Ожидание сигнала SIGUSR1...\n");
  while (1) {
    if (sigwait(&set, &signum) != 0) {
      perror("sigwait");
      exit(EXIT_FAILURE);
    }
    printf("Переход на следующую итеарцию\n");

    if (signum == SIGUSR1) {
      printf("Получен сигнал SIGUSR1\n");
    }
  }

  return 0;
}
