// requires additional changes to the code to make it work
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include <immintrin.h>

#define A(i, j) A[(i) * SIZE + (j)]
#define max(x, y) (((x) < (y)) ? (y) : (x))

static double gtod_ref_time_sec = 0.0;

/* Adapted from the bl2_clock() routine in the BLIS library */

double
dclock ()
{
    double the_time, norm_sec;
    struct timeval tv;
    gettimeofday (&tv, NULL);

    if (gtod_ref_time_sec == 0.0)
        gtod_ref_time_sec = (double)tv.tv_sec;

    norm_sec = (double)tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + tv.tv_usec * 1.0e-6;

    return the_time;
}

int
ge (double *A, size_t SIZE)
{
    register int i, j, k;

    register __m256d mm_multiplier;
    register __m256d r0, r1, rw0, rw1;

    for (k = 0; k < SIZE; k++)
    {
        register double c = A (k, k);

        for (i = k + 1; i < SIZE; i++)
        {

            register double multiplier = A (i, k) / c;
            mm_multiplier[0] = multiplier;
            mm_multiplier[1] = multiplier;
            mm_multiplier[2] = multiplier;
            mm_multiplier[3] = multiplier;

            for (j = k + 1; j < SIZE;)
            {
                if (j < max (SIZE - 8, 0))
                {

                    rw0 = _mm256_loadu_pd (&A (i, j + 0));
                    rw1 = _mm256_loadu_pd (&A (i, j + 4));

                    r0 = _mm256_loadu_pd (&A (k, j + 0));
                    r1 = _mm256_loadu_pd (&A (k, j + 4));

                    r0 = _mm256_mul_pd (r0, mm_multiplier);
                    r1 = _mm256_mul_pd (r1, mm_multiplier);

                    rw0 = _mm256_sub_pd (rw0, r0);
                    rw1 = _mm256_sub_pd (rw1, r1);

                    _mm256_storeu_pd (&A (i, j + 0), rw0);
                    _mm256_storeu_pd (&A (i, j + 4), rw1);

                    j += 8;
                }
                else
                {
                    A (i, j) = A (i, j) - A (k, j) * multiplier;
                    j++;
                }
            }
        }
    }
    return 0;
}

int
main (int argc, const char *argv[])
{
    int i, j, k, iret;
    double dtime;
    int SIZE = 1500;
    double *matrix = calloc (SIZE * SIZE, sizeof (double));

    srand (1);
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            matrix[j + SIZE * i] = rand ();

    dtime = dclock ();
    iret = ge (matrix, SIZE);
    dtime = dclock () - dtime;

    printf ("Time: %le \n", dtime);

    double check = 0.0;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            check = check + matrix[i + SIZE * j];

    printf ("Check: %le \n", check);
    fflush (stdout);
    free (matrix);

    return iret;
}
