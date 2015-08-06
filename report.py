#!/usr/bin/env python3

import sys
import json
import csv
import re

NAME_RE = re.compile(
        '^BM<' +
        '(?P<counter>\w+)<(?P<array>\w+)>' +
        ',\s*' +
        '(?P<type>\w+)_(?P<matrix>\w+)_(?P<height>\d+)x(?P<width>\d+)' +
        '_(?P<id>(\d|\w|_)*)' +
        '>$')

TIME_MULTIPLIER = 1e-6

def load_run_info(d):
    info = NAME_RE.match(d['name']).groupdict()
    info['cpu time ms'] = round(d['cpu_time'] * TIME_MULTIPLIER, 2)
    return info

def read_json(json_file=None):
    json_file = json_file or sys.stdin
    data = json.load(json_file)
    return [load_run_info(bench) for bench in data['benchmarks']]

def write_csv(runs, csv_file=None):
    csv_file = csv_file or sys.stdout
    writer = csv.DictWriter(csv_file,
            ['type', 'counter', 'array', 'matrix',
             'height', 'width', 'id', 'cpu time ms'])
    writer.writeheader()
    for r in runs:
        writer.writerow(r)

if __name__ == '__main__':
    runs = read_json()
    write_csv(runs)

