#include <papi.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "ge5.h"

#define A(i, j) A[(i) * n + j]
#define B(i, j) B[(i) * n + j]
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

void
ge_check (double *A, const size_t n)
{
    for (size_t k = 0; k < n; k++)
        for (size_t i = k + 1; i < n; i++)
            for (size_t j = k + 1; j < n; j++)
                A (i, j) -= A (k, j) * (A (i, k) / A (k, k));
}

int
main (int argc, const char *argv[])
{
    // Measure GFLOPS
    for (size_t n = 20; n < 1520; n += 20)
    {
        double t;
        double GFLOPS = (2.0 / 3.0) * n * n * n * 1.0e-09;
        double *A = malloc (n * n * sizeof (double));
        double *B = malloc (n * n * sizeof (double));
        double *_A = malloc (n * n * sizeof (double));

        // Generate random matrix
        srand (1);
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < n; j++)
                A (i, j) = rand ();

        // Copy matrix A to B to check correctness with naive algorithm
        memcpy (B, A, n * n * sizeof (double));

        ge_check (B, n);

        double checkB = 0.0;
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < n; j++)
                checkB += B (i, j);

        double best = 100000;

        for (size_t trial = 0; trial < 10; trial++)
        {
            memcpy (_A, A, n * n * sizeof (double));

            // Measure time
            t = dclock ();
            ge (_A, n);
            t = dclock () - t;
            if (t < best)
                best = t;

            double checkA = 0.0;
            for (size_t i = 0; i < n; i++)
                for (size_t j = 0; j < n; j++)
                    checkA += _A[i * n + j];

            if (checkA != checkB)
                printf ("ERROR %f %f\n", checkA, checkB);
        }
        printf ("[%ld, %le],\n", n, GFLOPS / best);

        // Check correctness

        free (A);
        free (B);
        free (_A);
    }

    return 0;
}