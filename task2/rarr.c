#include <stdio.h>
#include <stdlib.h>

int main()
{
  int N = 0;

  printf("Введите размер массива\n");
  scanf("%d", &N);

  int matrix[N];
  printf("Ввод: ");
  for (int i = 0; i < N; i++) {
    matrix[i] = i + 1;
    printf("%d ", matrix[i]);
  }
  putchar('\n');

  for (int i = 0; i < N / 2; i++) {
    int temp = matrix[i];
    matrix[i] = matrix[N - i - 1];
    matrix[N - i - 1] = temp;
  }

  printf("Вывод: ");

  for (int i = 0; i < N; i++) {
    printf("%d ", matrix[i]);
  }
  putchar('\n');

  return 0;
}