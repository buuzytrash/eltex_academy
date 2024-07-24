#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLEAR_COMMAND "clear"

void clear_terminal() { system(CLEAR_COMMAND); }

void enter_wait()
{
  char enter;

  printf("Нажмите Enter, чтобы продолжить.\n");
  while ((enter = getchar()) != '\n' && enter != EOF)
    ;
  getchar();
  clear_terminal();
}

void file_writting(const char* filename, const char* string)
{
  FILE* out_file;
  out_file = fopen(filename, "w");

  if (!out_file) {
    perror("Ошибка открытия файла для записи.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(out_file, "%s", string);

  printf("Запись завершилась!\n");

  fclose(out_file);
  enter_wait();
}

void file_reading(const char* filename)
{
  FILE* in_file;
  in_file = fopen(filename, "r");

  if (!in_file) {
    perror("Невозможно прочитать файл, попробуйте снова\n");
    exit(EXIT_FAILURE);
  }

  fseek(in_file, 0, SEEK_END);
  int file_size = ftell(in_file);
  printf("Строка из файла в обратном порядке: ");

  for (long i = file_size - 1; i >= 0; i--) {
    fseek(in_file, i, SEEK_SET);
    int ch = fgetc(in_file);
    if (ch != EOF) {
      putchar(ch);
    }
  }
  putchar('\n');

  fclose(in_file);
  enter_wait();
}

int main()
{
  const char* filename = "output.txt";
  const char* string = "String from file";

  clear_terminal();
  file_writting(filename, string);
  clear_terminal();
  file_reading(filename);
  clear_terminal();

  return 0;
}