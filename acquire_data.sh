#!/bin/bash

source ./op_dispatch_vars.sh

for operation in "${OPERATIONS[@]}"; do
    for size in "${SIZES[@]}"; do
        for (( i = 0 ; i < 30 ; i++)); do
            for (( rank = 0 ; rank < 64 ; rank++ )); do
                cat /scratch/sbird/tensor/"$i"/"$operation"_"$size"/OPERATION_rank_"$rank"_run_"$i".csv >> ./results/"$operation"_"$size".csv
                #if [ $? -ne 0 ]; then
                #    break
                #fi
            done
        done
    done
done

