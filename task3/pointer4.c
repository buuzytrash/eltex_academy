#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 16

int main()
{
  int str_len = STR_LEN;
  char* str = (char*)malloc((str_len + 1) * sizeof(char));
  char* sub_str = (char*)malloc((str_len + 1) * sizeof(char));
  if (str == NULL || sub_str == NULL) {
    printf("Ошибка распределения памяти!\n");
    return -1;
  }

  printf("Введите строку : ");

  int i = 0;
  char c;
  while ((c = getchar()) != '\n') {
    if (i == str_len - 1) {
      str_len *= 2;
      str = (char*)realloc(str, (str_len + 1) * sizeof(char));
      if (str == NULL) {
        printf("Ошибка распределения памяти!\n");
        return -1;
      }
    }
    str[i++] = c;
  }
  str[i] = '\0';

  printf("Введите подстроку размером не более 16 символов : ");
  fgets(sub_str, STR_LEN + 1, stdin);
  sub_str[sizeof(sub_str) - 1] = '\0';

  int sub_str_len = strlen(sub_str) - 1;
  i = 0;

  char* p = NULL;
  while (i < str_len - sub_str_len + 1) {
    int j = 0;
    while (j < sub_str_len && str[i + j] == sub_str[j]) {
      j++;
    }
    if (j == sub_str_len) {
      printf("Подстрока найдена!\n");
      p = str + i;
      break;
    }
    i++;
  }

  if (p == NULL) {
    printf("Подстрока не найдена!\n");
  }
  else {
    printf("Указатель на начало подстроки: %p - %c\n", p, *p);
  }

  free(str);
  free(sub_str);

  return 0;
}