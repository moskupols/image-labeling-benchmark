#!/usr/bin/env python3

import sys
import json
import csv
import re
import itertools
import os
import collections


def read_benchmarks_csv(csv_file=None):
    csv_file = csv_file or sys.stdin
    reader = csv.DictReader(csv_file)
    return list(reader)


COUNTER_MAP = {
    "Dfs": "DFS",
    "Dsu": "SUF",
}


def write_benchmarks_latex(runs):
    runs.sort(key=lambda r: (r['counter'], '2x2' in r['grid'], int(r['density'])))

    headers = ['counter', '2x2']
    densities = set()
    lines = []

    for (counter, cond), runs in itertools.groupby(runs, lambda r: (r['counter'], '2x2' in r['grid'])):
        runs = tuple(runs)
        counter = counter[:counter.index('Counter')]
        line = {
            headers[0]: COUNTER_MAP.get(counter, counter),
            headers[1]: 'yes' if cond else 'no',
        }
        for r in runs:
            dens = r['density']
            densities.add(dens)
            line[dens] = str(round(float(r['cpu time ms']), 1))
        lines.append(line)

    columns = len(headers) + len(densities)
    print(r'\begin{tabular}{| l | r | ' + 'r ' * (columns - 2) + '| }')

    print(r'\cline{' + '{}-{}'.format(3, columns) + '}')
    print(r'\multicolumn{' + str(len(headers)) + '}{c}{} & '
        + r'\multicolumn{' + str(len(densities)) + '}{|c|}{density}', end='\\\\\n')

    print('\\hline')
    headers.extend(map(str, sorted(map(int, densities))))
    print(' & '.join(headers), end='\\\\\n')
    print('\\hline')
    for line in lines:
        print(' & '.join([line.get(k, '') for k in headers]), end='\\\\\n')

    print('\\hline')
    print(r'\end{tabular}')


if __name__ == '__main__':
    runs = read_benchmarks_csv()
    write_benchmarks_latex(runs)

