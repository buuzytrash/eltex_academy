#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100

typedef struct
{
  char name[10];
  char second_name[10];
  char tel[10];
} abonent;

typedef enum
{
  DEFAULT,
  ADD_ABONENT,
  DELETE_ABONENT,
  FIND_ABONENT,
  SHOW_ABONENTS,
  EXIT
} menu_item;

void usage()
{
  printf("Введите пункт из меню ниже\n");
  printf("1) Добавить абонента\n");
  printf("2) Удалить абонента\n");
  printf("3) Поиск абнента по имени\n");
  printf("4) Вывод всех записей\n");
  printf("5) Выход\n\n");
}

int main(void)
{
  abonent abonents[N];
  int abonents_count = 0;
  int user_choice = DEFAULT;

  usage();

  while (1) {
    printf("%d\n", user_choice);

    if (user_choice == EXIT) {
      exit(EXIT_SUCCESS);
    }

    scanf("%d", &user_choice);
    switch (user_choice) {
      case (ADD_ABONENT): {
        if (abonents_count < N) {
          printf("Введите имя: ");
          scanf("%s", abonents[abonents_count].name);
          printf("Введите фамилию: ");
          scanf("%s", abonents[abonents_count].second_name);
          printf("Введите номер телефона: ");
          scanf("%s", abonents[abonents_count].tel);
          putchar('\n');

          abonents_count++;
        }
        else {
          printf("Список полон\n");
        }

        user_choice = DEFAULT;

        usage();
      }

      case (DELETE_ABONENT): {
        char remove_name[10];
        int flag = 0;

        printf("Введите имя абонента, которого хотите удалить\n ");
        scanf("%s", remove_name);

        for (int i = 0; i < N; i++) {
          if (strcmp(abonents[i].name, remove_name) == 0) {
            for (int j = i; j < N - 1; j++) {
              abonents[j] = abonents[j + 1];
            }
            abonents_count--;
            flag = 1;
            break;
          }
        }

        if (flag == 1) {
          printf("Абонент %s удален\n", remove_name);
        }
        else {
          printf("Абонент %s не найден\n", remove_name);
        }

        user_choice = DEFAULT;
      }
    }
  }

  return 0;
}