#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

void fill_matrices(float *a, float *b, int dim)
{
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (i == j) {
                a[i*dim + j] = 1;
                b[i*dim + j] = 1;
            } else if (i + 1 == j) {
                b[i*dim + j] = 1;
            } else if (j + 1 == i) {
                a[i*dim + j] = 1;
            }
        }
    }
}

int main(int argc, char **argv)
{
#ifndef DATA_SIZES
    printf("--- FAILED: no DATA_SIZES list defined. i need one or more data sizes to benchmark\n");
    return 0;
#endif
#ifndef NUM_DATA_SIZES
#define NUM_DATA_SIZES 1
    printf("--- WARN: NUM_DATA_SIZES not provided. only the first size in DATA_SIZES will be benchmarked\n");
#endif
    
    float *a, *b, *c;
    int data_sizes[] = { DATA_SIZES };
    float bench_timings[NUM_DATA_SIZES];

    for (int i = 0; i < NUM_DATA_SIZES; ++i) {
        int dim = data_sizes[i];

        a = (float*) calloc(dim * dim, sizeof(float));
        b = (float*) calloc(dim * dim, sizeof(float));
        c = (float*) calloc(dim * dim, sizeof(float));

        if (a == NULL || b == NULL || c == NULL) {
            fprintf(stderr, "failed to allocate enough memory for the matrices\n");
            return 1;
        }

        fill_matrices(a, b, dim);

        float tic = (float) clock() / CLOCKS_PER_SEC;
        matmul(a, b, c, dim, dim, dim); // naive matrix multiply
        float toc = (float) clock() / CLOCKS_PER_SEC;

        bench_timings[i] = toc - tic;

        free(a);
        free(b);
        free(c);
    }

    FILE *outfile = fopen("naive_bench.csv", "w");
    for (int i = 0; i < NUM_DATA_SIZES; ++i) {
        fprintf(outfile, "%d,%f\n", data_sizes[i], bench_timings[i]);
    }
    fclose(outfile);

    return 0;
}

