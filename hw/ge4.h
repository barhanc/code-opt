#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>
#include <omp.h>

#define A(i, j) A[(i) * n + j]

// Użycie openmp z dostrojoną liczbą wątków

void
ge (double *A, const size_t n)
{
    register size_t i, j, k;
    register double c, m;
    register __m256d m_vec, y_vec0, A_vec0;

    for (k = 0; k < n; k++)
    {
        c = A (k, k);

#pragma omp parallel for num_threads(8)
        for (i = k + 1; i < n; i++)
        {
            m = -A (i, k) / c;
            m_vec = _mm256_set1_pd (m);

            for (j = k + 1; j < n - 3; j += 4)
            {
                y_vec0 = _mm256_loadu_pd (&A (k, j + 0));
                A_vec0 = _mm256_loadu_pd (&A (i, j + 0));
                _mm256_storeu_pd (&A (i, j + 0), _mm256_fmadd_pd (m_vec, y_vec0, A_vec0));
            }

            for (; j < n; j++)
                A (i, j) += m * A (k, j);
        }
    }
}