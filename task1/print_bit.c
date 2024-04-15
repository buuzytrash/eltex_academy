#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Пример пользования программой
void usage()
{
  printf("Please enter the integer number\n");
  printf("Usage: ./a.out <num>\n Where <num> is your integer number\n");
}

// Вывод целого положительного числа в двоичном виде с использованием маски
void print_bit(int num)
{
  int space = 0;
  unsigned int mask = 1 << (sizeof(int) * 8 - 1);

  if (num < 0) {
    printf("Двоичное представление отрицательного числа: %d\n", num);
    num = ~(-num) + 1;
  }
  else {
    printf("Двоичное представление числа: %d\n", num);
  }

  while (mask) {
    if ((space % 8) == 0) {
      putchar(' ');
    }
    putchar((num & mask) ? '1' : '0');
    mask >>= 1;
    space++;
  }
}

int main(int argc, char** argv)
{
  int num = 0;

  if (argc < 2) {
    usage();
    return 1;
  }

  if (fmod(atof(argv[1]), 1.0) != 0.0) {
    printf("Вы ввели не целое число!\n");
    usage();
    return 2;
  }

  num = atoi(argv[1]);
  print_bit(num);
  putchar('\n');

  return 0;
}