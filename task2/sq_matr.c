#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N = 0;

    printf("Введите размер квадратной матрицы\n");
    scanf("%d", &N);

    int c = 1;
    int matrix[N][N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = c++;
            printf("%d\t", matrix[i][j]);
        }
        printf("\n\v");
    }
    return 0;
}