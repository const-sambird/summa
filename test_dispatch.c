#include "mpi.h"
#include "summa.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * Initialise matrices based on this rank
 */
void fill_matrices(float *a, float *b, int rank)
{
    // for testing, assume we've got a 2x2 processor grid (4x4 total)
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            a[i*2 + j] = rank;
            b[i*2 + j] = rank;
        }
    }
}

int main(int argc, char **argv)
{
#ifndef PROC_ROWS
    fprintf(stderr, "the processor architecture PROC_ROWS must be defined\n");
    return 1;
#endif
#ifndef PROC_COLS
    fprintf(stderr, "the processor architecture PROC_COLS must be defined\n");
    return 1;
#endif
    assert(PROC_ROWS == PROC_COLS && "the processor grid is not a square!");

    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Comm row_comm;
    int row_rank, row_size;
    MPI_Comm col_comm;
    int col_rank, col_size;

    if (argc != 4) {
        fprintf(stderr, "usage: %s [i] [k] [j]\n", argv[0]);
    }

    int i = atoi(argv[1]);
    int k = atoi(argv[2]);
    int j = atoi(argv[3]);

    int proc_row_num = world_rank / PROC_COLS;
    int proc_col_num = world_rank % PROC_COLS;
    
    int ib = i / PROC_ROWS;
    int kb = k / PROC_COLS;
    int jb = j / PROC_COLS;
    int numblocks = PROC_ROWS;

    MPI_Comm_split(MPI_COMM_WORLD, proc_row_num, world_rank, &row_comm);
    MPI_Comm_split(MPI_COMM_WORLD, proc_col_num, world_rank, &col_comm);

    // two inputs, one output
    float *a = (float*) calloc(ib * kb, sizeof(float));
    float *b = (float*) calloc(kb * jb, sizeof(float));
    float *c = (float*) calloc(ib * jb, sizeof(float));

    // plus three work matrices
    // i could probably reuse one but it'd be more complicated
    float *abuf = (float*) calloc(ib * kb, sizeof(float));
    float *bbuf = (float*) calloc(kb * jb, sizeof(float));
    float *cbuf = (float*) calloc(ib * jb, sizeof(float));
 
    fill_matrices(a, b, world_rank);

    printf("i'm rank %d at co-ordinates (%d, %d). my block of a:\n", world_rank, proc_row_num, proc_col_num);
    print_matrix(a, ib, kb);
    printf("my block of b:\n");
    print_matrix(b, kb, jb);

    summa(a, b, c, abuf, bbuf, cbuf, ib, kb, jb, numblocks, proc_row_num, proc_col_num, row_comm, col_comm);

    MPI_Finalize();

    printf("my block of c:\n");
    print_matrix(c, ib, jb);

    free(abuf);
    free(bbuf);
    free(cbuf);
    free(a);
    free(b);
    free(c);
}
