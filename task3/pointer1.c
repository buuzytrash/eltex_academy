#include <stdio.h>

void change_third_byte(int *num, int new_value)
{
    // Указатель на третий байт числа
    unsigned char *third_byte = (unsigned char *)num + 2;
    // Изменяем значение третьего байта
    *third_byte = (unsigned char)new_value;
}

int main()
{
    int num;
    int new_value;

    printf("Введите целое положительное число: ");
    scanf("%d", &num);

    if (num < 0)
    {
        printf("Вы ввели отрицательное число!\n");
        return 1;
    }

    printf("Введите новое значение для третьего байта (0-255): ");
    scanf("%d", &new_value);

    // Проверка на допустимость нового значения, так как изменяем только 1 байт числа
    if (new_value < 0 || new_value > 255)
    {
        printf("Ошибка: новое значение должно быть в диапазоне от 0 до 255.\n");
        return 2;
    }

    printf("Исходное число: %d\n", num);
    change_third_byte(&num, new_value);
    printf("Измененное число: %d\n", num);

    return 0;
}