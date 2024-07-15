#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  printf("\t%s\n", cur_abon.name);
  printf("\t%s\n", cur_abon.second_name);
  printf("\t%s\n\n", cur_abon.tel);
}

void add_abonent(abonent** abonents, int* abonents_count)
{
  *abonents = realloc(*abonents, (*abonents_count + 1) * sizeof(abonent));
  if (*abonents == NULL) {
    printf("Ошибка выделения памяти\n");
    return;
  }

  printf("Введите имя: ");
  scanf("%s", (*abonents)[*abonents_count].name);
  printf("Введите фамилию: ");
  scanf("%s", (*abonents)[*abonents_count].second_name);
  printf("Введите номер телефона: ");
  scanf("%s", (*abonents)[*abonents_count].tel);
  putchar('\n');

  (*abonents_count)++;
}

void delete_abonent(abonent** abonents, int* abonents_count)
{
  char remove_name[10];
  int index = -1;

  printf("Введите имя абонента, которого хотите удалить\n ");
  scanf("%s", remove_name);

  for (int i = 0; i < *abonents_count; i++) {
    if (strcmp((*abonents)[i].name, remove_name) == 0) {
      index = i;
      break;
    }
  }

  if (index != -1) {
    abonent deleted_abonent = (*abonents)[index];
    for (int i = index; i < *abonents_count - 1; i++) {
      (*abonents)[i] = (*abonents)[i + 1];
    }
    (*abonents)[*abonents_count] = deleted_abonent;
    free((*abonents)[*abonents_count]);
    *abonents_count -= 1;
    *abonents = realloc(*abonents, *abonents_count * sizeof(abonent));
    printf("Абонент %s удален\n", remove_name);
  }
  else {
    printf("Абонент %s не найден\n", remove_name);
  }
}

void find_abonent(abonent* abonents, int abonents_count)
{
  char find_name[10];
  size_t flag = 0;
  printf("Введите имя абонента, которого хотите найти\n");
  scanf("%s", find_name);
  for (int i = 0; i < abonents_count; i++) {
    if (strcmp(abonents[i].name, find_name) == 0) {
      flag = 1;
      print_abonent(abonents[i]);
    }
  }

  if (flag == 0) {
    printf("Данных этого абонента нет в справочнике\n");
  }
}

void show_abonents(abonent* abonents, int abonents_count)
{
  for (int i = 0; i < abonents_count; i++) {
    print_abonent(abonents[i]);
  }
}

int main(void)
{
  abonent* abonents = NULL;
  int abonents_count = 0;
  int user_choice = DEFAULT;

  while (1) {
    usage();

    scanf("%d", &user_choice);

    switch (user_choice) {
      case (ADD_ABONENT): {
        add_abonent(&abonents, &abonents_count);
        user_choice = DEFAULT;
        break;
      }

      case (DELETE_ABONENT): {
        delete_abonent(&abonents, &abonents_count);
        user_choice = DEFAULT;
        break;
      }

      case (FIND_ABONENT): {
        find_abonent(abonents, abonents_count);
        break;
      }

      case (SHOW_ABONENTS): {
        show_abonents(abonents, abonents_count);
        user_choice = DEFAULT;
        break;
      }

      case (EXIT): {
        free(abonents);
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