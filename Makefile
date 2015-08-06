CC=g++
COMMON_FLAGS=-Wall -Wextra --std=c++11 -fstack-protector -fsanitize=address -fsanitize=undefined

D_FLAGS=$(COMMON_FLAGS) -O0 -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread
BENCH_FLAGS=-lbenchmark -pthread

SOURCES = matrix.* int_array.* profile_counter.h dfs_counter.h dsu.* dsu_counter.h
SOURCES += testgen.h counters_common.*
COMPILED_SOURCES=matrix.cxx int_array.cxx counters_common.cxx dsu.cxx

ULIMITED=ulimit -s 6100500 &&

BENCH_FILTER=DsuCounter|<IntArray
REPORT_FILE=report.csv

run: main
	./main

main: debug

debug: $(SOURCES)
	$(CC) $(D_FLAGS) -o main main.cxx $(COMPILED_SOURCES)

release: $(SOURCES) main.cxx
	$(CC) $(R_FLAGS) -o main main.cxx $(COMPILED_SOURCES)

test: counters-test
	$(ULIMITED) ./counters-test

counters-test: counters-test.cxx $(SOURCES)
	$(CC) $(R_FLAGS) counters-test.cxx $(COMPILED_SOURCES) $(GTEST_FLAGS) -o $@

bench: benchmark
	$(ULIMITED) ./benchmark --benchmark_filter='$(BENCH_FILTER)'

benchmark: benchmark.cxx $(SOURCES)
	$(CC) $(R_FLAGS) benchmark.cxx $(BENCH_FLAGS) $(COMPILED_SOURCES) -o $@

report: $(REPORT_FILE)

$(REPORT_FILE): report.py benchmark
	$(ULIMITED) ./benchmark --benchmark_format=json --benchmark_filter='$(BENCH_FILTER)' | ./report.py >$@

clean:
	rm -f main

