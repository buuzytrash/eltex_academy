#include <stdio.h>
#include <stdlib.h>

#include "add.h"
#include "div.h"
#include "mul.h"
#include "sub.h"

#define CLEAR_COMMAND "clear"

typedef enum
{
  DEFAULT,
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  EXIT
} menu_item;

void clear_terminal() { system(CLEAR_COMMAND); }

void usage()
{
  printf(
      "Добро пожаловать в простейшее приложение Калькулятор.\nВыберите пункт "
      "из меню ниже:\n");
  printf("1) Сложение\n");
  printf("2) Вычитание\n");
  printf("3) Умножение\n");
  printf("4) Деление\n");
  printf("5) Выход\n\n");
}
void enter_wait()
{
  char enter;

  printf("Нажмите Enter, чтобы продолжить.\n");
  while ((enter = getchar()) != '\n' && enter != EOF)
    ;
  getchar();
  clear_terminal();
}

int main(void)
{
  clear_terminal();
  int user_choice = DEFAULT;
  int x = 0, y = 0;

  while (1) {
    usage();
    scanf("%d", &user_choice);
    clear_terminal();

    switch (user_choice) {
      case ADDITION:

        x = 0, y = 0;
        printf("Вы выбрали сложение. Введите 2 целых числа:\n");
        scanf("%d %d", &x, &y);
        printf("Результат:\n%d + %d = %d\n", x, y, addition(x, y));

        enter_wait();

        break;
      case SUBTRACTION:

        x = 0, y = 0;
        printf("Вы выбрали вычитание. Введите 2 целых числа:\n");
        scanf("%d %d", &x, &y);
        printf("Результат:\n %d - %d = %d\n", x, y, subtraction(x, y));

        enter_wait();

        break;
      case MULTIPLICATION:

        x = 0, y = 0;
        printf("Вы выбрали умножение. Введите 2 целых числа:\n");
        scanf("%d %d", &x, &y);
        printf("Результат:\n %d * %d = %d\n", x, y, multiplication(x, y));

        enter_wait();

        break;
      case DIVISION:

        float x = 0, y = 0;
        printf("Вы выбрали деление. Введите 2 целых числа:\n");
        scanf("%f %f", &x, &y);
        printf("Результат:\n %.2f / %.2f = %.2f\n", x, y, division(x, y));

        enter_wait();

        break;
      case EXIT:
        clear_terminal();
        exit(EXIT_SUCCESS);
        break;
      default:
        printf("Такого пункта меню нет\nПопробуйте снова\n");
        break;
    }
  }

  return 0;
}