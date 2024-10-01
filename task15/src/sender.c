#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  if (argc != 3) {
    fprintf(stderr,
            "Введите: %s <signal> <pid>\n SIGUSR1 - 10\n SIGINT - 2\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  int sig = atoi(argv[1]);

  pid_t pid = atoi(argv[2]);

  switch (sig) {
    case (2): {
      if (kill(pid, SIGINT) == -1) {
        perror("kill");
        return 1;
      }
      printf("Сигнал SIGINT отправлен процессу с pid %d\n", pid);
    } break;

    case (10): {
      if (kill(pid, SIGUSR1) == -1) {
        perror("kill");
        return 1;
      }
      printf("Сигнал SIGUSR1 отправлен процессу с pid %d\n", pid);
    }

    default:
      break;
  }

  return 0;
}