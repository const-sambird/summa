from matplotlib import pyplot as plt
from statistics import fmean
import pickle
import os

operations = ['short_bcast', 'long_bcast', 'builtin_bcast']
sizes = ['2', '4', '8']
lengths = [i*1024 for i in range(1, 9)]

results = {}

with open('results/results.pkl', 'rb') as infile:
    results = pickle.load(infile)

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
    plt.title('SUMMA timing: %s physical nodes' % size)
    plt.xlabel('Matrix dimension size (# floats)')
    plt.ylabel('Time (seconds)')
    plt.savefig('results/%s.png' % size)
    plt.close()

print('Complete')
