#!/bin/bash

source ./op_dispatch_vars.sh

for operation in "${OPERATIONS[@]}"; do
    for size in "${SIZES[@]}"; do
        for (( i = 0 ; i < 30 ; i++)); do
            if ! test -f /scratch/sbird/tensor/"$i"/"$operation"_"$size"/OPERATION_rank_0_run_"$i".csv; then
                echo "--- FAILED: $operation, $size nodes, run $i"
                rm /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size"_stdout.txt
                rm /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size"_stderr.txt
                sbatch /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
            fi
        done
    done
done

