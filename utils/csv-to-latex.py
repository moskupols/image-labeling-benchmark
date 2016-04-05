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

def regroup_runs(runs):
    grids = ['no', 'view', 'yes']
    gridfunc = lambda g: 1 if 'View' in g else 2 if '2x2' in g else 0
    runs.sort(key=lambda r: (
        'Decis' in r['counter'],
        r['counter'],
        gridfunc(r['grid']),
        int(r['density'])))

    densities = set()
    lines = []

    for (counter, compression), runs in itertools.groupby(runs, lambda r: (r['counter'], gridfunc(r['grid']))):
        runs = tuple(runs)
        line = {
            'method': COUNTER_MAP.get(counter, counter),
            '2x2': grids[compression],
        }
        for r in runs:
            dens = int(r['density'])
            densities.add(dens)
            line[dens] = float(r['cpu time ms'])
        lines.append(line)
    return lines, sorted(densities)

METHOD_COLOR = {
    'DFS': 'red',
    'SUF': 'green',
    "SUF2": 'blue',
    'DTSUF': 'purple'
}

METHOD_SHAPE = {
    'DFS': '.',
    'SUF': '.',
    "SUF2": '.',
    'DTSUF': '.'
}

COND_SHAPE = {
    'no': '-',
    'view': '--',
    'yes': ':'
}

COND_LEGEND = {
    'no': '',
    'view': '+view',
    'yes': '+2x2'
}

def plot_benchmarks(runs, out_file='output.eps', dpi=1200):
    from matplotlib import pyplot as plt
    from matplotlib.font_manager import FontProperties

    fontP = FontProperties()
    fontP.set_size('small')

    lines, densities = regroup_runs(runs)

    for line in lines:
        method = line['method']
        condensation = line['2x2']
        ys = [line[x] for x in densities]

        plt.plot(densities, ys,
                METHOD_SHAPE[method] + COND_SHAPE[condensation],
                color=METHOD_COLOR[method],
                label=method + COND_LEGEND[condensation])

    plt.xlabel('density, %')
    plt.ylabel('CPU time, ms')

    if '2000' not in out_file:
        lgd = plt.legend(loc=2, prop=fontP)
    else:
        lgd = plt.legend(loc=2, bbox_to_anchor=(1, 1), prop=fontP)
    plt.savefig(out_file,
            bbox_extra_artists=(lgd,), bbox_inches='tight',
            format=out_file[-3:], dpi=dpi)


def write_table(lines, densities):
    HEADERS = ['method', '2x2']

    def wrapsize(s):
        #  return '\\scriptsize{' + str(s) + '}'
        return str(s)

    columns = len(HEADERS) + len(densities)

    column_types = ['r'] * len(densities)
    print(r'\begin{tabularx}{\linewidth}{| l  r  @{\extracolsep{\fill}}'
            + ' '.join(column_types) + '| }')

    print(r'\cline{' + '{}-{}'.format(3, columns) + '}')
    print(r'\multicolumn{' + str(len(HEADERS)) + '}{c}{} & '
        + r'\multicolumn{' + str(len(densities)) + '}{|c|}{density}', end='\\\\\n')

    print('\\hline')
    print(' & '.join(HEADERS + list(map(wrapsize, densities))), end='\\\\\n')
    print('\\hline')
    for _, counter_lines in itertools.groupby(lines, lambda li: li[HEADERS[0]]):
        for line in counter_lines:
            values = [wrapsize(my_round(line[d])) for d in densities]
            line = list(map(wrapsize, map(line.get, HEADERS))) + values
            print(' & '.join(line), end='\\\\\n')
        print('\\hline')

    print(r'\end{tabularx}')

def write_benchmarks_latex(runs):
    lines, densities = regroup_runs(runs)

    if len(densities) < 12:
        write_table(lines, densities)
    else:
        boundary = len(densities) // 2 + 1
        write_table(lines, densities[:boundary])
        print('\n\n\\qquad\n\n')
        write_table(lines, densities[boundary:])



if __name__ == '__main__':
    runs = read_benchmarks_csv()
    dpi = 1200
    if '--dpi' in sys.argv:
        dpi = int(sys.argv[sys.argv.index('--dpi')+1])
    if '--plot' in sys.argv:
        plot_benchmarks(runs, sys.argv[sys.argv.index('--plot')+1], dpi=dpi)
    write_benchmarks_latex(runs)

