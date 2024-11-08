#include "mpi.h"
#include "summa.h"
#include "matrix.h" 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define STR_(X) #X
#define STR(X) STR_(X)

/**
 * Initialise matrices based on this rank
 */
void fill_matrices(float *a, float *b, int rank, int ib, int jb)
{
    int global_row = (rank / PROC_COLS) * ib;
    int global_col = (rank % PROC_COLS) * ib;
    for (int i = global_row; i < ib; ++i) {
        for (int j = global_col; j < jb; ++j) {
            if (i == j) {
                a[i * ib + j] = 1;
                b[i * ib + j] = 1;
            } else if (i + 1 == j) {
                b[i * ib + j] = 1;
            } else if (j + 1 == i) {
                a[i * ib + j] = 1;
            }
        }
    }
}

int main(int argc, char **argv)
{
#ifndef PROC_ROWS
    fprintf(stderr, "--- FAILED: the processor architecture PROC_ROWS must be defined\n");
    return 0;
#endif
#ifndef DATA_SIZES
    printf("--- FAILED: no DATA_SIZES list defined. i need one or more data sizes to benchmark\n");
    return 0;
#endif
#ifndef NUM_DATA_SIZES
#define NUM_DATA_SIZES 1
    printf("--- WARN: NUM_DATA_SIZES not provided. only the first size in DATA_SIZES will be benchmarked\n");
#endif
#ifndef RUN_INDEX
#define RUN_INDEX 0
    printf("--- WARN: RUN_INDEX not defined. are we not looping? defaulting to 0.\n");
#endif

    assert(PROC_ROWS == PROC_COLS && "the processor grid is not a square!");

    int data_sizes[] = { DATA_SIZES };
    double bench_timings[NUM_DATA_SIZES];

    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Comm row_comm;
    int row_rank, row_size;
    MPI_Comm col_comm;
    int col_rank, col_size;

    int proc_row_num = world_rank / PROC_COLS;
    int proc_col_num = world_rank % PROC_COLS;

    // two inputs, one output
    float *a, *b, *c;
    // three work matrices
    float *abuf, *bbuf, *cbuf;

    for (int i = 0; i < NUM_DATA_SIZES; ++i) {
        int data_size = data_sizes[i];

        int ib = data_size / PROC_ROWS;
        int kb = data_size / PROC_COLS;
        int jb = data_size / PROC_COLS;
        int numblocks = PROC_ROWS;

        MPI_Comm_split(MPI_COMM_WORLD, proc_row_num, world_rank, &row_comm);
        MPI_Comm_split(MPI_COMM_WORLD, proc_col_num, world_rank, &col_comm);

        a = (float*) calloc(ib * kb, sizeof(float));
        b = (float*) calloc(kb * jb, sizeof(float));
        c = (float*) calloc(ib * jb, sizeof(float));

        abuf = (float*) calloc(ib * kb, sizeof(float));
        bbuf = (float*) calloc(kb * jb, sizeof(float));
        cbuf = (float*) calloc(ib * jb, sizeof(float));

        if (a == NULL || b == NULL || c == NULL || abuf == NULL || bbuf == NULL || cbuf == NULL) {
            printf("--- FAILED: rank %d (%d, %d) couldn't allocate sufficient memory for size %d\n", world_rank, proc_row_num, proc_col_num, data_size);
            return 1;
        }

        fill_matrices(a, b, world_rank, ib, jb);

        MPI_Barrier(MPI_COMM_WORLD);

        double tic = MPI_Wtime();
        summa(a, b, c, abuf, bbuf, cbuf, ib, kb, jb, numblocks, proc_row_num, proc_col_num, row_comm, col_comm);
        double toc = MPI_Wtime();

        bench_timings[i] = toc - tic;
        free(a); free(b); free(c);
        free(abuf); free(bbuf); free(cbuf);
        MPI_Comm_free(&row_comm); MPI_Comm_free(&col_comm);
        MPI_Barrier(MPI_COMM_WORLD);
    }
 
    MPI_Finalize();

    char filename[50];
    snprintf(filename, 50, STR(OPERATION) "_rank_%d_run_%d.csv", world_rank, RUN_INDEX);
    FILE *outfile = fopen(filename, "w");
    for (int i = 0; i < NUM_DATA_SIZES; ++i) {
        fprintf(outfile, "%d,%f\n", data_sizes[i], bench_timings[i]);
    }
    fclose(outfile);

    printf("--- SUCCESS: rank %d completed all processes\n", world_rank);

    return 0;
}

