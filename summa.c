#include "summa.h"
#include "matrix.h"
#include "bcast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void summa(float *a,
           float *b,
           float *c,
           float *abuf, float *bbuf, float *cbuf,
           int ib, int kb, int jb, int numblocks,
           int proc_row, int proc_col,
           MPI_Comm row_comm, MPI_Comm col_comm)
{
    // store references to the original pointers
    // so we don't need to memcpy anything to
    // the buffers when broadcasting - we can just
    // reference *a and *b 
    float *abuf_ref = abuf;
    float *bbuf_ref = bbuf;
    for (int blk = 0; blk < numblocks; ++blk) {
        // zero out cbuf
        memset(cbuf, 0, ib * jb * sizeof(float));

        if (proc_col == blk)
            abuf = a;
        if (proc_row == blk)
            bbuf = b;
        
        bcast(abuf, ib * kb, MPI_FLOAT, blk, row_comm, proc_col, numblocks);
        bcast(bbuf, kb * jb, MPI_FLOAT, blk, col_comm, proc_row, numblocks);

        matmul(abuf, bbuf, cbuf, ib, kb, jb);
        matadd(cbuf, c, c, ib, jb);

        if (proc_row == blk)
            abuf = abuf_ref;
        if (proc_col == blk)
            bbuf = bbuf_ref;
    }
}
