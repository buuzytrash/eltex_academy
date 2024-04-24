#include <stdio.h>

#define SIZE 10

int main()
{
  int arr[SIZE];
  for (int i = 1; i <= SIZE; i++) {
    arr[i - 1] = i;
  }

  int* p = arr;
  for (int i = 0; i < SIZE; i++) {
    printf("%d ", *(p + i));
  }
  putchar('\n');

  return 0;
}
