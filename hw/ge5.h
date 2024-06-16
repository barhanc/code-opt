#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>
#include <omp.h>

#define A(i, j) A[(i) * n + j]
#define min(x, y) (((x) < (y)) ? (x) : (y))

// Użycie openmp

void
ge (double *A, const int n)
{
    const size_t s2 = 8;
    const size_t s1 = 64;

    double *x = malloc (n * sizeof (double));
    double *y = malloc (n * sizeof (double));

    register double c, m;
    register __m256d vec_m, vec_y1, vec_A1;

    register int i, j, k, ii, jj;

    for (k = 0; k < n; k++)
    {
        c = A (k, k);
        for (i = k + 1; i < n; i++)
            x[i - k - 1] = -(A (i, k) / c), y[i - k - 1] = A (k, i);

#pragma omp parallel for num_threads(8)
        for (ii = k + 1; ii < n; ii += s2)
            for (jj = k + 1; jj < n; jj += s1)
                for (i = ii; i < min (ii + s2, n); i++)
                {
                    j = jj;
                    m = x[i - k - 1];
                    vec_m = _mm256_set1_pd (m);

                    for (; j < min (jj + s1, n) - 4; j += 4)
                    {
                        vec_y1 = _mm256_loadu_pd (y + j - k - 1);
                        vec_A1 = _mm256_loadu_pd (&A (i, j));
                        _mm256_storeu_pd (&A (i, j), _mm256_fmadd_pd (vec_m, vec_y1, vec_A1));
                    }

                    for (; j < min (jj + s1, n); j++)
                        A (i, j) += m * y[j - k - 1];
                }
    }

    free (x), free (y);
}