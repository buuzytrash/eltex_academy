#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Пример пользования программой
void usage()
{
  printf("Please enter the positive integer number\n");
  printf(
      "Usage: ./a.out <num>\n Where <num> is your positive integer number\n");
}

// Вывод целого положительного числа в двоичном виде с использованием маски
void check_bit(int num)
{
  printf("Количество единиц в числе %d: ", num);
  short count_of_one = 0;

  while (num) {
    count_of_one += num & 1;
    num >>= 1;
  }
  printf("%d\n", count_of_one);
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

  if ((num = atoi(argv[1])) < 0) {
    printf("Вы ввели отрицательное число!\n");
    usage();
    return 3;
  }
  check_bit(num);

  return 0;
}