#!/bin/bash

rm -rf /scratch/sbird/tensor
module load OpenMPI

source ./op_dispatch_vars.sh

# for division
sixtyfour=64

for operation in "${OPERATIONS[@]}"; do
    for size in "${SIZES[@]}"; do
        echo "Compiling source for $operation on $size nodes"
        for (( i = 0 ; i < 30 ; i++ )); do
            mkdir -p /scratch/sbird/tensor/"$i"/"$operation"_"$size"
            mpicc -DDATA_SIZES="$DATA_SIZES" -DNUM_DATA_SIZES="$NUM_DATA_SIZES" -DBCAST="$operation" -DRUN_INDEX="$i" -DPROC_COLS=8 -DPROC_ROWS=8 run_timing.c summa.c matrix.c bcast.c -o /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".mpi
            echo "#!/bin/bash" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
            echo "#SBATCH --partition=normal"  >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
            echo "#SBATCH --exclusive" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
            echo "#SBATCH --nodes=$size" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
            echo "#SBATCH --ntasks=64" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --ntasks-per-node=$(( sixtyfour / size ))" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
            echo "#SBATCH --mem=16G" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --output=""$operation"_"$size""_stdout.txt" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --error=""$operation"_"$size""_stderr.txt" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --time=00:30:00" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --job-name=""$operation"_"$size"_"$i" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --mail-user=sam.bird@ou.edu" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --mail-type=ALL" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "#SBATCH --chdir=/scratch/sbird/tensor/"$i"/""$operation"_"$size" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "module load OpenMPI" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
	    echo "mpirun ""$operation"_"$size"".mpi" >> /scratch/sbird/tensor/"$i"/"$operation"_"$size"/"$operation"_"$size".sh
        done
    done
done

