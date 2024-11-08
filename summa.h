#ifndef _SUMMA_H
#define _SUMMA_H

#include "mpi.h"

void summa(float *a,
           float *b,
           float *c,
           float *abuf, float *bbuf, float *cbuf,
           int ib, int kb, int jb, int numblocks,
           int proc_row, int proc_col,
           MPI_Comm row_comm, MPI_Comm col_comm);
#endif
