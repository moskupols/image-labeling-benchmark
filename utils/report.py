#!/usr/bin/env python3

import sys
import json
import csv
import re

NAME_RE = re.compile(
        '^BM<' +
        '(?P<counter_signature>(?P<counter>\w+)(<(?P<array>\w*)>)?)' +
        ',\s*' +
        '(?P<test_signature>'
        '(?P<type>\w+?)_(?P<matrix>\w+?)(_(?P<height>\d+)x(?P<width>\d+))?' +
        '(_(?P<id>(\d|\w|_)*))?' +
        ')>$')

TIME_MULTIPLIER = 1e-6

def load_run_info(d):
    info = NAME_RE.match(d['name']).groupdict()
    info['cpu time ms'] = round(d['cpu_time'] * TIME_MULTIPLIER, 2)
    info['real time ms'] = round(d['real_time'] * TIME_MULTIPLIER, 2)
    return info

def read_benchmarks_json(json_file=None):
    json_file = json_file or sys.stdin
    data = json.load(json_file)
    return [load_run_info(bench) for bench in data['benchmarks']]

def write_benchmarks_csv(runs, csv_file=None):
    csv_file = csv_file or sys.stdout
    writer = csv.DictWriter(csv_file,
            ['type', 'counter', 'array', 'matrix',
             'height', 'width', 'id', 'cpu time ms'],
            extrasaction='ignore')
    writer.writeheader()
    for r in runs:
        writer.writerow(r)

def update_best(runs, filepath):
    try:
        with open(filepath, 'r') as fin:
            best_runs = json.load(fin)
    except FileNotFoundError:
        best_runs = {}

    for r in runs:
        t = r['test_signature']
        if t not in best_runs or best_runs[t]['time'] > r['real time ms']:
            best_runs[t] = {
                'time': r['real time ms'],
                'counter_signature': r['counter_signature'],
            }
    with open(filepath, 'w') as fout:
        json.dump(best_runs, fout, sort_keys=True, indent=2)

if __name__ == '__main__':
    runs = read_benchmarks_json()
    if '--update-best' in sys.argv:
        update_best(runs, 'best.json')
    write_benchmarks_csv(runs)

