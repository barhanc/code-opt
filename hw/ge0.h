#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>

#define A(i, j) A[(i) * n + j]
#define B(i, j) B[(i) * n + j]

void
ge_naive (double *A, const size_t n)
{
    register size_t i, j, k;

    for (k = 0; k < n; k++)
        for (i = k + 1; i < n; i++)
            for (j = k + 1; j < n; j++)
                A (i, j) -= A (k, j) * (A (i, k) / A (k, k));
}