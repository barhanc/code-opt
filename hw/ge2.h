#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>
#include <stdio.h>

#define A(i, j) A[(i) * n + j]

typedef double v4f64_t __attribute__ ((vector_size (4 * sizeof (double))));

void
ge (double *A, const size_t n)
{
    double *x = malloc (n * sizeof (double));
    double *y = malloc (n * sizeof (double));

    for (size_t k = 0; k < n; k++)
    {
        register double c = A (k, k);

        for (size_t i = k + 1; i < n; i++)
        {
            x[i - k - 1] = A (i, k) / c;
            y[i - k - 1] = A (k, i);
        }

        // TODO: Check how BLAS `ger` function is implemented. This is literally it.
        for (size_t i = k + 1; i < n; i++)
            for (size_t j = k + 1; j < n; j++)
                A (i, j) -= x[i - k - 1] * y[j - k - 1];
    }

    free (x);
    free (y);
}