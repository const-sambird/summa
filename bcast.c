#include "bcast.h"
#include <stdio.h>

#ifndef BCAST
#define BCAST builtin_bcast
#endif

// supporting operations for the bcast collectives
void mst_bcast(void *buf, int size, int rank, int root, MPI_Comm comm, MPI_Datatype type, int left, int right)
{
    if (left == right) return;
    int mid = (left + right) / 2;
    int dest;                                                                                              
        
    if (root <= mid) {
        dest = right;                                                                                      
    } else {
        dest = left;
    }   
            
    if (rank == root)                                                                                      
        MPI_Send(buf, size, type, dest, 0, comm); 

    if (rank == dest)
        MPI_Recv(buf, size, type, root, 0, comm, MPI_STATUS_IGNORE);


    if (rank <= mid && root <= mid) {
        mst_bcast(buf, size, rank, root, comm, type, left, mid);
    } else if (rank <= mid && root > mid) {
        mst_bcast(buf, size, rank, dest, comm, type, left, mid);
    } else if (rank > mid && root <= mid) {
        mst_bcast(buf, size, rank, dest, comm, type, mid + 1, right);
    } else if (rank > mid && root > mid) {
        mst_bcast(buf, size, rank, root, comm, type, mid + 1, right);
    }
}

void mst_scatter(float *buf, int size, int rank, int root, MPI_Comm comm, MPI_Datatype type, int left, int right)
{
    if (left == right) return;
    int mid = (left + right) / 2;
    int dest;

    if (root <= mid) {
        dest = right;
    } else {
        dest = left;
    }
    
    if (root <= mid) {
        if (rank == root) {
            MPI_Send(buf + ((mid + 1) * size), (right - (mid + 1) + 1) * size, type, dest, 0, comm);
        }
        if (rank == dest) {
            MPI_Recv(buf + ((mid + 1) * size), (right - (mid + 1) + 1) * size, type, root, 0, comm, MPI_STATUS_IGNORE);
        }
    } else { 
        if (rank == root) {
            MPI_Send(buf + (left * size), (mid - left + 1) * size, type, dest, 0, comm);
        }
        if (rank == dest) {
            MPI_Recv(buf + (left * size), (mid - left + 1) * size, type, root, 0, comm, MPI_STATUS_IGNORE);
        }
    } 
        
    if (rank <= mid && root <= mid) {
        mst_scatter(buf, size, rank, root, comm, type, left, mid);
    } else if (rank <= mid && root > mid) {
        mst_scatter(buf, size, rank, dest, comm, type, left, mid);
    } else if (rank > mid && root <= mid) {
        mst_scatter(buf, size, rank, dest, comm, type, mid + 1, right);
    } else if (rank > mid && root > mid) {
        mst_scatter(buf, size, rank, root, comm, type, mid + 1, right);
    }
}

int get_subset_size(int idx, int size, int num_ranks)
{
    if (idx + 1 == num_ranks)
        return size - (idx * (size / num_ranks));
    return size / num_ranks;
}

void bkt_allgather(float *vec, int size, int rank, int num_ranks, MPI_Comm comm, MPI_Datatype type)
{       
    int prev = rank - 1; 
    if (prev < 0) prev = num_ranks - 1;
    int next = rank + 1;
    if (next == num_ranks) next = 0;
            
    int current = rank;
        
    MPI_Request request; 
    
    for (int i = 0; i < num_ranks - 1; ++i) {
        MPI_Isend(vec + (current * (size / num_ranks)), get_subset_size(current, size, num_ranks), type, next, 0, comm, &request);
    
        current = current - 1; 
        if (current < 0) current = num_ranks - 1;
        MPI_Recv(vec + (current * (size / num_ranks)), get_subset_size(current, size, num_ranks), type, prev, 0, comm, MPI_STATUS_IGNORE);

        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
}

void short_bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs)
{
    mst_bcast(buf, count, rank, root, comm, datatype, 0, num_procs - 1);
}

void long_bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs)
{
    mst_scatter((float*) buf, count / num_procs, rank, root, comm, datatype, 0, num_procs - 1);
    bkt_allgather((float*) buf, count, rank, num_procs, comm, datatype);
}

void builtin_bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs)
{
    MPI_Bcast(buf, count, datatype, root, comm);
}

void bcast(void *buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm, int rank, int num_procs)
{
    BCAST(buf, count, datatype, root, comm, rank, num_procs);
}

