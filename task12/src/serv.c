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
  const char buf[] = "Hi!";

  if (mkfifo("my_fifo", 0666) == -1) {
    perror("Ошибка канала");
    exit(EXIT_FAILURE);
  }

  fd = open("my_fifo", O_WRONLY);
  if (fd == -1) {
    perror("Ошибка открытия файла");
    exit(EXIT_FAILURE);
  }

  if (write(fd, buf, sizeof(buf)) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  close(fd);

  return 0;
}