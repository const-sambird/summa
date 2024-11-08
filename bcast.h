#ifndef _BCAST_H
#define _BCAST_H

#include "mpi.h"

void short_bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs);
void long_bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs);
void builtin_bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs);
void bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs);

#endif

