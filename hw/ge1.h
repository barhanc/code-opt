#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>

#define A(i, j) A[(i) * n + j]

// Umieszcenie zmiennych w rejestrach
// Jednowymiamorwa tablica indeksowana poprzez makro

void
ge (double *A, const int n)
{
    register int i, j, k;
    register double c, m;

    for (k = 0; k < n; k++)
    {
        c = A (k, k);
        for (i = k + 1; i < n; i++)
        {
            m = -A (i, k) / c;
            for (j = k + 1; j < n; j++)
                A (i, j) += m * A (k, j);
        }
    }
}