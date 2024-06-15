#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>

#define A(i, j) A[(i) * n + j]

void
ge (double *A, const size_t n)
{
    for (size_t k = 0; k < n; k++)
        for (size_t i = k + 1; i < n; i++)
            for (size_t j = k + 1; j < n; j++)
                A (i, j) -= A (k, j) * (A (i, k) / A (k, k));
}