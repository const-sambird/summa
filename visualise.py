from matplotlib import pyplot as plt
from statistics import fmean
import csv
import os
import pickle

RESULT_ROOT = '/scratch/sbird/tensor'
operations = ['short_bcast', 'long_bcast', 'builtin_bcast']
sizes = ['2', '4', '8']
num_runs = 30
num_ranks = 64
lengths = [i*1024 for i in range(1, 9)]

results = {}

for i in range(num_runs):
    print('Loading data: %d/%d' % (i, num_runs))
    for operation in operations:
        for size in sizes:
            this_result = '%s_%s' % (operation, size)
            for rank in range(num_ranks):
                if not os.path.isfile('%s/%d/%s/%s_rank_%d_run_%d.csv' % (RESULT_ROOT, i, this_result, 'OPERATION', rank, i)):
                    continue
                with open('%s/%d/%s/%s_rank_%d_run_%d.csv' % (RESULT_ROOT, i, this_result, 'OPERATION', rank, i)) as csvfile:
                    reader = csv.DictReader(csvfile, fieldnames=['length', 'time'])
                    for row in reader:
                        if this_result not in results:
                            results[this_result] = {}
                        if row['length'] not in results[this_result]:
                            results[this_result][row['length']] = []
                        results[this_result][row['length']].append(row['time'])
                    csvfile.close()

pickle.dump(results, open('results/results.pkl', 'wb'))

for size in sizes:
    print('Creating plot for %s nodes' % size)
    ax = plt.figure().add_subplot(111)
    for operation in operations:
        this_result = '%s_%s' % (operation, size)
        xs = []
        ys = []
        for length in lengths:
            xs.append(length)
            ys.append(fmean([float(x) for x in results[this_result][str(length)]]))
        ax.scatter(xs, ys, label=operation)

        plt.legend(loc='upper left')
        plt.title('SUMMA timing: %s physical nodes' % (operation, size))
        plt.xlabel('Matrix dimension size (# floats)')
        plt.ylabel('Time (seconds)')
        plt.savefig('results/%s_%s.png' % (operation, size))
        plt.close()

print('Complete')
