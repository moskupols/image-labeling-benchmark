#!/usr/bin/env python3

import sys
import json
import csv
import re
import itertools
import os
import collections


def my_round(x):
    from math import log10, floor

    x = float(x)
    x = round(x, -int(floor(log10(abs(x)))) + 1)
    return round(x) if x - round(x) < 1e-4 and x >= 10 else x


def read_benchmarks_csv(csv_file=None):
    csv_file = csv_file or sys.stdin
    reader = csv.DictReader(csv_file)
    return list(reader)


COUNTER_MAP = {
    "DecisionTreeDsuCounter": "DTSUF",
    "DfsCounter": "DFS",
    "DsuCounter": "SUF",
    "TwolineDsuCounter": "SUF2"
}

def write_benchmarks_latex(runs):
    grids = ['no', 'view', 'yes']
    gridfunc = lambda g: 1 if 'View' in g else 2 if '2x2' in g else 0
    runs.sort(key=lambda r: (
        'Decis' in r['counter'],
        r['counter'],
        gridfunc(r['grid']),
        int(r['density'])))

    headers = ['method', '2x2']
    densities = set()
    lines = []

    for (counter, compression), runs in itertools.groupby(runs, lambda r: (r['counter'], gridfunc(r['grid']))):
        runs = tuple(runs)
        line = {
            headers[0]: COUNTER_MAP.get(counter, counter),
            headers[1]: grids[compression],
        }
        for r in runs:
            dens = r['density']
            densities.add(dens)
            line[dens] = str(my_round(r['cpu time ms']))
        lines.append(line)

    columns = len(headers) + len(densities)
    print(r'\begin{tabularx}{\textwidth}{| l  r  @{\extracolsep{\fill}}'
            + 'r ' * (columns - 2) + '| }')

    print(r'\cline{' + '{}-{}'.format(3, columns) + '}')
    print(r'\multicolumn{' + str(len(headers)) + '}{c}{} & '
        + r'\multicolumn{' + str(len(densities)) + '}{|c|}{density}', end='\\\\\n')

    print('\\hline')
    headers.extend(map(str, sorted(map(int, densities))))
    print(' & '.join(headers), end='\\\\\n')
    print('\\hline')
    for _, counter_lines in itertools.groupby(lines, lambda li: li[headers[0]]):
        for line in counter_lines:
            print(' & '.join([line.get(k, '') for k in headers]), end='\\\\\n')
        print('\\hline')

    print(r'\end{tabularx}')


if __name__ == '__main__':
    runs = read_benchmarks_csv()
    write_benchmarks_latex(runs)

