# summa

This repository benchmarks the performance of the Scalable Universal Matrix Multiplication Algorithm, or SUMMA (R van de Geijn & J Watts, 1995). It builds on my earlier benchmarks for collective communication operations, which may be found here ([repository](https://github.com/const-sambird/collectives), [report](https://sambird.org/reports/collective_comm.pdf)).

This repository forms part of my final project for my master's degree at the University of Oklahoma. The report may be found [here](https://sambird.org/reports/summa.pdf).

The implementation of SUMMA itself is the same for each benchmark (see [`summa.c`](./summa.c)), but differs in the implementation of the `broadcast` collective operation that SUMMA uses.

```
void summa(...)
{
    for (each block within my row/column of processors)
    {
        BROADCAST(ith block of A within my row of processors)
        BROADCAST(ith block of B within my column of processors)
        my block of C += MATRIX_MULTIPLY(ith block of A and B)
    }
}
```

Note in the above pseudocode, `MATRIX_MULTIPLY` refers to naïve (schoolbook) matrix multiplication.

I am benchmarking the performance of SUMMA when using:

* A minimum spanning tree broadcast, optimised for short vectors ('short')
* A minimum spanning tree scatter, followed by a bucket allgather, optimised for long vectors ('long')
* OpenMPI's built-in `MPI_Bcast` function ('builtin')

I test these against matrices of variable sizes, from 1024 by 1024 to 8192 by 8192. These are run 30 times each on different physical processor architectures: while in every case the processes are arranged in a 64 by 64 grid, those logical processors are distributed across 2, 4, and 8 compute nodes.

The computing for this project was performed at the OU Supercomputing Center for Education & Research (OSCER) at the University of Oklahoma.

