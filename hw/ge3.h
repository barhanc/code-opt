#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>

#define A(i, j) A[(i) * n + j]

// Pozbycie się if else

void
ge (double *A, const size_t n)
{
    register size_t i, j, k;
    register double c, m;
    register __m256d m_vec, y_vec0, A_vec0;

    for (k = 0; k < n; k++)
    {
        c = A (k, k);

        for (i = k + 1; i < n; i++)
        {
            m = -A (i, k) / c;
            m_vec = _mm256_set1_pd (m);

            for (j = k + 1; j < n - 4; j += 4)
            {
                y_vec0 = _mm256_loadu_pd (&A (k, j));
                A_vec0 = _mm256_loadu_pd (&A (i, j));
                _mm256_storeu_pd (&A (i, j), _mm256_fmadd_pd (m_vec, y_vec0, A_vec0));
            }

            for (; j < n; j++)
                A (i, j) += m * A (k, j);
        }
    }
}