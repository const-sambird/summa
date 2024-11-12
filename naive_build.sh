#!/bin/bash
module load GCC

source ./op_dispatch_vars.sh

sizes="1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024"

rm -f naive.x
gcc -DDATA_SIZES="$sizes" -DNUM_DATA_SIZES="30" bench_naive.c matrix.c -o naive.x

