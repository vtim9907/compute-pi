#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define ROUND 100
#define SAMPLE_NUM 1000

double calc_ci (double sample_time[])
{
    int j, count = 0;
    double average = 0.0, total_time = 0.0, sigma, upper, lower, conf_interval = 0.0;
    for (j = 0; j < SAMPLE_NUM; ++j) {
        average += sample_time[j];
        total_time += sample_time[j] * sample_time[j];
    }
    average /= SAMPLE_NUM;
    sigma = sqrt(total_time / SAMPLE_NUM - average * average);
    upper = average + 1.96 * sigma;
    lower = average - 1.96 * sigma;
    for (j = 0; j < SAMPLE_NUM; ++j) {
        if (sample_time[j] > lower && sample_time[j] < upper) {
            conf_interval += sample_time[j];
            count++;
        }
    }
    return conf_interval /= count;
}

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int i, j, loop = ROUND;
    double sample_time[SAMPLE_NUM];

    // Baseline
    for (j = 0; j < SAMPLE_NUM; j++) {
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_baseline(N);
        }
        clock_gettime(CLOCK_ID, &end);
        sample_time[j] = (double) (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    printf("%lf,", calc_ci(sample_time));

    // OpenMP with 2 threads
    for (j = 0; j < SAMPLE_NUM; j++) {
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_openmp(N, 2);
        }
        clock_gettime(CLOCK_ID, &end);
        sample_time[j] = (double) (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    printf("%lf,", calc_ci(sample_time));

    // OpenMP with 4 threads
    for (j = 0; j < SAMPLE_NUM; j++) {
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_openmp(N, 4);
        }
        clock_gettime(CLOCK_ID, &end);
        sample_time[j] = (double) (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    printf("%lf,", calc_ci(sample_time));

    // AVX SIMD
    for (j = 0; j < SAMPLE_NUM; j++) {
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_avx(N);
        }
        clock_gettime(CLOCK_ID, &end);
        sample_time[j] = (double) (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    printf("%lf,", calc_ci(sample_time));

    // AVX SIMD + Loop unrolling
    for (j = 0; j < SAMPLE_NUM; j++) {
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            compute_pi_avx_unroll(N);
        }
        clock_gettime(CLOCK_ID, &end);
        sample_time[j] = (double) (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    printf("%lf,", calc_ci(sample_time));

    // Leibniz OpenMP
    for (j = 0; j < SAMPLE_NUM; j++) {
        clock_gettime(CLOCK_ID, &start);
        for (i = 0; i < loop; i++) {
            leibniz_openmp(N, 4);
        }
        clock_gettime(CLOCK_ID, &end);
        sample_time[j] = (double) (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }

    printf("%lf\n", calc_ci(sample_time));

    return 0;
}
