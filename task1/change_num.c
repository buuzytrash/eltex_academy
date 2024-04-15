#include <stdio.h>

void print(unsigned int num)
{
  unsigned int shift = 1 << (sizeof(int) * 8 - 1);
  int space = 0;

  while (shift) {
    if ((space % 8) == 0) {
      putchar(' ');
    }
    putchar((num & shift) ? '1' : '0');
    shift >>= 1;
    space++;
  }
  putchar('\n');
}
void change_third_byte(unsigned int num, unsigned int new_value)
{
  unsigned int mask = 0xff00ffff;

  new_value = new_value << 16;
  num = num & mask;
  num = num | new_value;

  printf("Измененное число: %d\n", num);

  print(num);
}

int main()
{
  unsigned int num;
  unsigned int new_value;

  printf("Введите целое положительное число: ");
  scanf("%u", &num);

  if (num < 0) {
    printf("Вы ввели отрицательное число!\n");
    return 1;
  }

  printf("Введите новое значение для третьего байта (0-255): ");
  scanf("%u", &new_value);

  // Проверка на допустимость нового значения, так как изменяем только 1 байт
  // числа
  if (new_value < 0 || new_value > 255) {
    printf("Ошибка: новое значение должно быть в диапазоне от 0 до 255.\n");
    return 2;
  }

  printf("Исходное число: %d\n", num);
  print(num);

  change_third_byte(num, new_value);

  return 0;
}