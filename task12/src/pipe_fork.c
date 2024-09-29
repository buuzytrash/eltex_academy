#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  pid_t pid;
  int status;
  int fd[2];
  char buf[64];

  if (pipe(fd)) {
    perror("Ошибка канала");
    EXIT_FAILURE;
  }

  pid = fork();

  if (pid < 0) {
    perror("Ошибка создания процесса");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) {
    printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    close(fd[0]);

    sprintf(buf, "%s", "Hi!");
    write(fd[1], buf, strlen(buf) + 1);

    exit(EXIT_SUCCESS);
  }
  else {
    printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());
    close(fd[1]);
    read(fd[0], buf, sizeof(buf));
    printf("Сообщение дочернего процесса: %s\n", buf);

    wait(&status);
    if (WIFEXITED(status)) {
      printf("Дочерний процесс завершился с кодом %d\n", WEXITSTATUS(status));
    }
    else {
      printf("Дочерний процесс завершился аварийно\n");
    }
  }

  return 0;
}