#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>

#define A(i, j) A[(i) * n + j]
#define max(x, y) (((x) < (y)) ? (y) : (x))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// Obliczanie elementów wynikowych macierzy w blokach s2 x s1

void
ge (double *A, const int n)
{
    const int s2 = 36;
    const int s1 = 16 * 3;

    register double c, m;
    register __m256d m_vec;
    register __m256d y_vec0, y_vec1, y_vec2, y_vec3;
    register __m256d A_vec0, A_vec1, A_vec2, A_vec3;

    register int i, j, k, ii, jj;

    for (k = 0; k < n; k++)
    {
        c = A (k, k);

        for (ii = k + 1; ii < n; ii += s2)
            for (jj = k + 1; jj < n; jj += s1)
                for (i = ii; i < min (ii + s2, n); i++)
                {
                    m = -A (i, k) / c;
                    m_vec = _mm256_set1_pd (m);

                    for (j = jj; j < min (jj + s1, n);)
                    {
                        if (j < max (n - 16, 0))
                        {
                            y_vec0 = _mm256_loadu_pd (&A (k, j));
                            y_vec1 = _mm256_loadu_pd (&A (k, j + 4));
                            y_vec2 = _mm256_loadu_pd (&A (k, j + 8));
                            y_vec3 = _mm256_loadu_pd (&A (k, j + 12));

                            A_vec0 = _mm256_loadu_pd (&A (i, j));
                            A_vec1 = _mm256_loadu_pd (&A (i, j + 4));
                            A_vec2 = _mm256_loadu_pd (&A (i, j + 8));
                            A_vec3 = _mm256_loadu_pd (&A (i, j + 12));

                            _mm256_storeu_pd (&A (i, j), _mm256_fmadd_pd (m_vec, y_vec0, A_vec0));
                            _mm256_storeu_pd (&A (i, j + 4), _mm256_fmadd_pd (m_vec, y_vec1, A_vec1));
                            _mm256_storeu_pd (&A (i, j + 8), _mm256_fmadd_pd (m_vec, y_vec2, A_vec2));
                            _mm256_storeu_pd (&A (i, j + 12), _mm256_fmadd_pd (m_vec, y_vec3, A_vec3));

                            j += 16;
                        }
                        else
                        {
                            A (i, j) += m * A (k, j);
                            j++;
                        }
                    }
                }
    }
}
