#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N = 0, M = 0;
    printf("Введите размер матрицы N и M\n");
    scanf("%d%d", &N, &M);

    int arr[N][M];

    int i_begin = 0, i_end = 0, j_begin = 0, j_end = 0;
    int k = 1, i = 0, j = 0;

    while (k <= N * M)
    {
        arr[i][j] = k;
        // двигаемся вправо
        if (i == i_begin && j < M - j_end - 1)
            ++j;
        // двигаемся вниз
        else if (j == M - j_end - 1 && i < N - i_end - 1)
            ++i;
        // двигаемся влево
        else if (i == N - i_end - 1 && j > j_begin)
            --j;
        // двигаемся вверх
        else
            --i;

        // начинаем новый проход по спирали
        if ((i == i_begin + 1) && (j == j_begin) && (j_begin != M - j_end - 1))
        {
            ++i_begin;
            ++i_end;
            ++j_begin;
            ++j_end;
        }
        ++k;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            printf("%d\t", arr[i][j]);
        putchar('\n');
    }
    return 0;
}