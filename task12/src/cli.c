#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  int fd;
  const char buf[10];
  fd = open("my_fifo", O_RDONLY);

  if (fd == -1) {
    perror("Ошибка открытия файла");
    exit(EXIT_FAILURE);
  }

  if (read(fd, buf, sizeof(buf)) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  // Выводим строку на экран
  printf("Получено от сервера: %s\n", buf);

  // Закрываем канал
  close(fd);

  // Удаляем канал
  if (unlink("my_fifo") == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }

  return 0;
}