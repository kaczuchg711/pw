#include <stdio.h>

void matrix_multiplication(const int *A,const int *B, int *C,int n,int m,int p)
{
//    n - wiersze A
//    m - kolumny A wiersze m
//    p - kolumny B
    int sum = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int k = 0; k < p; ++k)
        {
            for (int j = 0; j < m; ++j)
            {
                sum += *(B + k + (p * j)) * *(A + j + ((n + 1) * i));
            }
            *(C + k + ((n + 1) * (i) + i)) = sum;
            sum = 0;
        }
    }
}

int main()
{
//    liczba wierszy prawej mniejsza niż liczba kolumn z prawej
    int A[3][4] = {{1, 2, 3, 4},
                   {5, 6, 7, 8},
                   {9, 8, 7, 6}};
    int B[4][5] = {{4, 3, 2, 1, 2},
                   {3, 4, 5, 6, 7},
                   {8, 9, 8, 7, 6},
                   {5, 4, 3, 2, 1}};
    int C[3][5];
    matrix_multiplication((int *) A, (int *)B, (int *)C, 3, 4, 5);

    return 0;
}
