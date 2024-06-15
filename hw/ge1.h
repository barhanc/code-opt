#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>

#define A(i, j) A[(i) * n + j]
#define B(i, j) B[(i) * n + j]
#define max(x, y) (((x) < (y)) ? (y) : (x))

void
ge (double *A, const size_t n)
{
    register int i, j, k;

    register __m256d m;
    register __m256d r0, r1, rw0, rw1;

    for (k = 0; k < n; k++)
    {
        register double c = A (k, k);

        for (i = k + 1; i < n; i++)
        {

            register double multiplier = A (i, k) / c;
            m = _mm256_set1_pd (multiplier);

            for (j = k + 1; j < n;)
            {
                if (__builtin_expect (j < max (n - 8, 0), 1))
                {

                    rw0 = _mm256_loadu_pd (&A (i, j + 0));
                    rw1 = _mm256_loadu_pd (&A (i, j + 4));

                    r0 = _mm256_loadu_pd (&A (k, j + 0));
                    r1 = _mm256_loadu_pd (&A (k, j + 4));

                    r0 = _mm256_mul_pd (r0, m);
                    r1 = _mm256_mul_pd (r1, m);

                    rw0 = _mm256_sub_pd (rw0, r0);
                    rw1 = _mm256_sub_pd (rw1, r1);

                    _mm256_storeu_pd (&A (i, j + 0), rw0);
                    _mm256_storeu_pd (&A (i, j + 4), rw1);

                    j += 8;
                }
                else
                {
                    A (i, j) -= A (k, j) * multiplier;
                    j++;
                }
            }
        }
    }
}