#include "matrix.h"
#include <stdio.h>

/**
 * Simple (arithmetic) matrix multiplication algorithm
 * 
 * C[i, j] = A[i, k] * B[k, j]
 * 
 * @param a an i by k matrix
 * @param b a k by j matrix
 * @param c the i by j output matrix, initially zeroed
 * @param i the major dimension of input matrix a
 * @param k the common dimension of matrices a and b
 * @param j the major dimension of input matrix b
 */
void matmul(float *a, float *b, float *c, int i, int k, int j)
{
    for (int ii = 0; ii < i; ++ii) {
        for (int jj = 0; jj < j; ++jj) {
            for (int kk = 0; kk < k; ++kk) {
                c[ii*j + jj] += a[ii*k + kk] * b[kk*j + jj];
            }
        }
    }
}

/**
 * Simple (arithmetic) matrix addition algorithm
 * 
 * C[i, j] = A[i, j] + B[i, j]
 */
void matadd(float *a, float *b, float *c, int i, int j)
{
    for (int ii = 0; ii < i; ++ii) {
        for (int jj = 0; jj < j; ++jj) {
            c[ii*j + jj] = a[ii*j + jj] + b[ii*j + jj];
        }
    }
}

void print_matrix(float *mat, int i, int j)
{
    for (int row = 0; row < i; ++row) {
        for (int col = 0; col < j; ++col) {
            printf("%.2f ", mat[row*j + col]);
        }
        printf("\n");
    }
}
