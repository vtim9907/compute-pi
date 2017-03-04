#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "computepi.h"

#define STANDARD_PI acos(-1.0)
#define N 300000000

int main (int argc, char const *argv[])
{
    int i;
    double err;
    int method = atoi(argv[1]);

    if (method == 0) {
        for (i = 10000000; i <= N; i += 5000000) {
            printf("%d,",i);
            err = STANDARD_PI - compute_pi_openmp(i, 4);
            printf("%.25lf,",err);
            err = STANDARD_PI - leibniz_openmp(i, 4);
            printf("%.25lf\n",err);
        }
    }
    if (method == 1) {
        for (i = 10000000; i <= N; i += 5000000) {
            printf("%d,",i);
            err = STANDARD_PI - compute_pi_openmp(i, 4);
            printf("%.25lf\n",err);
        }
    }
    if (method == 2) {
        for (i = 10000000; i <= N; i += 5000000) {
            printf("%d,",i);
            err = STANDARD_PI - leibniz_openmp(i, 4);
            printf("%.25lf\n",err);
        }
    }

    return 0;
}
