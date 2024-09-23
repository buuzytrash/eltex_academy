#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  pid_t pid1, pid2, pid3, pid4, pid5;

  pid1 = fork();

  if (pid1 < 0) {
    perror("Ошибка создания процесса1");
    exit(EXIT_FAILURE);
  }
  else if (pid1 == 0) {
    printf(
        "Процесс1 (PID %d) процесса (PPID %d) запущен\n", getpid(), getppid());

    pid3 = fork();

    if (pid3 < 0) {
      perror("Ошибка создания процесса3");
      exit(EXIT_FAILURE);
    }
    else if (pid3 == 0) {
      printf("Процесс3 (PID %d) процесса1 (PPID %d) запущен\n",
             getpid(),
             getppid());
      sleep(2);
      printf("Процесс3 (PID %d) процесса1 (PPID %d) завершен\n",
             getpid(),
             getppid());
      exit(EXIT_SUCCESS);
    }
    else {
      pid4 = fork();

      if (pid4 < 0) {
        perror("Ошибка создания процесса4");
        exit(EXIT_FAILURE);
      }
      else if (pid4 == 0) {
        printf("Процесс4 (PID %d) процесса1 (PPID %d) запущен\n",
               getpid(),
               getppid());
        sleep(4);
        printf("Процесс4 (PID %d) процесса1 (PPID %d) завершен\n",
               getpid(),
               getppid());
        exit(EXIT_SUCCESS);
      }
      else {
        printf(
            "Родительский процесс1 (PID %d) ожидает завершения дочерних "
            "процессов\n",
            getpid());

        waitpid(pid3, NULL, 0);
        // printf("Процесс3 родительского процесса1 (PID %d) завершен\n",
        //        getpid());

        waitpid(pid4, NULL, 0);
        // printf("Процесс4 родительского процесса1 (PID %d) завершен\n",
        //        getpid());
      }
    }

    printf(
        "Процесс1 (PID %d) процесса (PPID %d) завершен\n", getpid(), getppid());
    exit(EXIT_SUCCESS);
  }
  else {
    pid2 = fork();

    if (pid2 < 0) {
      perror("Ошибка создания процесса2");
      exit(EXIT_FAILURE);
    }
    else if (pid2 == 0) {
      // Код, выполняемый во втором дочернем процессе
      printf("Процесс2 (PID %d) процесса (PPID %d) запущен\n",
             getpid(),
             getppid());

      pid5 = fork();

      if (pid5 < 0) {
        perror("Ошибка создания процесса5");
        exit(EXIT_FAILURE);
      }
      else if (pid5 == 0) {
        printf("Процесс5 (PID %d) процесса2 (PPID %d) запущен\n",
               getpid(),
               getppid());
        sleep(2);
        printf("Процесс5 (PID %d) процесса2 (PPID %d) завершен\n",
               getpid(),
               getppid());
        exit(EXIT_SUCCESS);
      }
      else {
        printf(
            "Родительский процесс2 (PID %d) ожидает завершения дочернего "
            "процесса\n",
            getpid());

        waitpid(pid5, NULL, 0);
        // printf("Процесс5 родительского процесса2 (PID %d) завершен\n",
        //        getpid());
      }

      printf("Процесс2 (PID %d) процесса (PPID %d) завершен\n",
             getpid(),
             getppid());
      exit(EXIT_SUCCESS);
    }
    else {
      printf(
          "Родительский процесс (PID %d) ожидает завершения дочерних "
          "процессов\n",
          getpid());

      waitpid(pid1, NULL, 0);
      //   printf("Процесс1 родительского процесса (PID %d) завершен\n",
      //   getpid());

      waitpid(pid2, NULL, 0);
      //   printf("Процесс2 родительского процесса (PID %d) завершен\n",
      //   getpid());

      printf("Родительский процесс (PID %d) завершен\n", getpid());
    }
  }

  return 0;
}