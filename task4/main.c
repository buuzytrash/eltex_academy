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

void print_abonent(abonent cur_abon)
{
  printf("%s\n", cur_abon.name);
  printf("%s\n", cur_abon.second_name);
  printf("%s\n\n", cur_abon.tel);
}

int main(void)
{
  abonent abonents[N];
  int abonents_count = 0;
  int user_choice = DEFAULT;

  while (1) {
    usage();

    scanf("%d", &user_choice);

    switch (user_choice) {
      case (ADD_ABONENT): {
        printf("%d\n", user_choice);

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

        break;
      }

      case (DELETE_ABONENT): {
        printf("%d\n", user_choice);

        char remove_name[10];
        int flag = 0;

        printf("Введите имя абонента, которого хотите удалить\n ");
        scanf("%s", remove_name);

        for (int i = 0; i < N; i++) {
          if (strcmp(abonents[i].name, remove_name) == 0) {
            // for (int j = i; j < N - 1; j++) { // Изначательно хотелось, чтобы
            // при удалении абонента, остальные абоненты сдвигались наверх на
            // свободную ячейку, но будем следовать ТЗ
            //   abonents[j] = abonents[j + 1];
            // }

            strcpy(abonents[i].name, "0");
            strcpy(abonents[i].second_name, "0");
            strcpy(abonents[i].tel, "0");

            // abonents_count--;
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
        break;
      }

      case (FIND_ABONENT): {
        char find_name[10];
        printf("Введите имя абонента, которого хотите найти\n");
        scanf("%s", find_name);
        for (int i = 0; i < abonents_count; i++) {
          if (strcmp(abonents[i].name, find_name) == 0) {
            print_abonent(abonents[i]);
          }
        }
        break;
      }

      case (SHOW_ABONENTS): {
        for (int i = 0; i < abonents_count; i++) {
          print_abonent(abonents[i]);
        }

        user_choice = DEFAULT;
        break;
      }

      case (EXIT): {
        exit(EXIT_SUCCESS);
        break;
      }

      default: {
        printf("Такого пункта меню нет\nПопробуйте снова\n");
        user_choice = DEFAULT;
        break;
      }
    }
  }

  return 0;
}