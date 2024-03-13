#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N = 0;
    printf("Введите размер матрицы N\n");
    scanf("%d", &N);

    int matrix[N][N];
    int a = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N - a; j++)
        {
            matrix[i][j] = 1;
            printf("%d ", matrix[i][j]);
        }
        for (int j = 0; j < a; j++)
        {
            matrix[i][j] = 0;
            printf("%d ", matrix[i][j]);
        }
        a++;
        putchar('\n');
    }

    return 0;
}