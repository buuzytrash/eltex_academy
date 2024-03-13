#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N = 0;

    printf("Введите размер массива\n");
    scanf("%d", &N);

    int c = 1;
    int matrix[N];
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &matrix[i]);
    }
    for (int i = N - 1; i >= 0; i--)
    {
        printf("%d ", matrix[i]);
    }
    putchar('\n');

    return 0;
}