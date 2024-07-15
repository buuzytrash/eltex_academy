#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 10
#define CLEAR_COMMAND "clear"

typedef struct
{
  char name[NAME_LENGTH];
  char second_name[NAME_LENGTH];
  char tel[NAME_LENGTH];
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

void clear_terminal() { system(CLEAR_COMMAND); }

void usage()
{
  printf("Введите пункт из меню ниже\n");
  printf("1) Добавить абонента\n");
  printf("2) Удалить абонента\n");
  printf("3) Поиск абонента по имени\n");
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
  char remove_name[NAME_LENGTH];
  int found = 0;

  printf("Введите имя абонента, которого хотите удалить: ");
  scanf("%s", remove_name);

  for (int i = 0; i < *abonents_count; i++) {
    if (strcmp((*abonents)[i].name, remove_name) == 0) {
      found = 1;
      (*abonents_count)--;
      if (i < *abonents_count) {
        (*abonents)[i] = (*abonents)[*abonents_count];
      }
      *abonents = realloc(*abonents, (*abonents_count) * sizeof(abonent));
      if (*abonents == NULL && *abonents_count != 0) {
        printf("Ошибка выделения памяти\n");
        return;
      }
      printf("Абонент %s удален\n", remove_name);
      break;
    }
  }

  if (!found) {
    printf("Абонент %s не найден\n", remove_name);
  }
}

void find_abonent(abonent* abonents, int abonents_count)
{
  char find_name[NAME_LENGTH];
  int found = 0;

  printf("Введите имя абонента, которого хотите найти: ");
  scanf("%s", find_name);

  for (int i = 0; i < abonents_count; i++) {
    if (strcmp(abonents[i].name, find_name) == 0) {
      print_abonent(abonents[i]);
      found = 1;
      break;
    }
  }

  if (!found) {
    printf("Не удалось найти информацию о данном абоненте\n");
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
      case ADD_ABONENT:
        clear_terminal();
        add_abonent(&abonents, &abonents_count);
        clear_terminal();
        break;
      case DELETE_ABONENT:
        clear_terminal();
        delete_abonent(&abonents, &abonents_count);
        clear_terminal();
        break;
      case FIND_ABONENT:
        clear_terminal();
        find_abonent(abonents, abonents_count);
        break;
      case SHOW_ABONENTS:
        clear_terminal();
        show_abonents(abonents, abonents_count);
        break;
      case EXIT:
        free(abonents);
        exit(EXIT_SUCCESS);
        break;
      default:
        printf("Такого пункта меню нет\nПопробуйте снова\n");
        break;
    }
  }

  return 0;
}