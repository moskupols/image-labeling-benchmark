#!/usr/bin/env python3

import sys
import json
import csv
import re

NAME_RE = re.compile(
        '^BM<' +
        '(?P<counter_signature>(?P<counter>\w+)(<(?P<params>(\w+,\s*))?(?P<grid>\w*)?>)?)' +
        ',\s*' +
        '(?P<test_signature>'
        '(?P<type>\w+?)_(?P<matrix>\w+?)(_(?P<height>\d+)x(?P<width>\d+))?' +
        '(_(?P<id>(\d|\w|_)*))?' +
        ')>$')

TIME_MULTIPLIER = 1e-6

def load_run_info(d):
    info = NAME_RE.match(d['name']).groupdict()
    info['cpu time ms'] = round(d['cpu_time'] * TIME_MULTIPLIER, 4)
    info['real time ms'] = round(d['real_time'] * TIME_MULTIPLIER, 4)
    return info

def read_benchmarks_json(json_file=None):
    json_file = json_file or sys.stdin
    data = json.load(json_file)
    return [load_run_info(bench) for bench in data['benchmarks']]

def write_benchmarks_csv(runs, csv_file=None):
    csv_file = csv_file or sys.stdout
    writer = csv.DictWriter(csv_file,
            ['type', 'counter', 'params', 'grid', 'matrix',
             'height', 'width', 'id', 'cpu time ms'],
            extrasaction='ignore')
    writer.writeheader()
    for r in runs:
        writer.writerow(r)

def choose_best(runs):
    stripped_runs = [
        (r['test_signature'],
           { 'time': r['cpu time ms'],
             'counter_signature': r['counter_signature']})
        for r in runs]
    ret = {}
    for r in stripped_runs:
        old = ret.get(r[0])
        if not old or old['time'] > r[1]['time']:
            ret[r[0]] = r[1]
    return ret

def update_best(new_bests, filepath):
    try:
        with open(filepath, 'r') as fin:
            old_bests = json.load(fin)
    except FileNotFoundError:
        old_bests = {}

    for t, run in new_bests.items():
        if t not in old_bests or old_bests[t]['time'] > run['time'] * 1.1:
            old_bests[t] = run

    with open(filepath, 'w') as fout:
        json.dump(old_bests, fout, sort_keys=True, indent=2)

if __name__ == '__main__':
    runs = read_benchmarks_json()
    if '--update-best' in sys.argv:
        update_best(choose_best(runs), 'best.json')
    write_benchmarks_csv(runs)

