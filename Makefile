CC=g++
COMMON_FLAGS=--std=c++11
D_FLAGS=$(COMMON_FLAGS) -O0 -fstack-protector -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread

SOURCES=matrix.cxx matrix.h counter.h dfs_counter.h
COMPILED_SOURCES=matrix.cxx

BENCH_FLAGS=-lbenchmark -pthread

run: main
	./main

main: debug

debug: $(SOURCES)
	$(CC) $(D_FLAGS) -o main main.cxx $(COMPILED_SOURCES) 

release: $(SOURCES) main.cxx
	$(CC) $(R_FLAGS) -o main main.cxx $(COMPILED_SOURCES)

test: test-counters

test-counters: counters-test
	./counters-test

counters-test: counters-test.cxx $(SOURCES)
	$(CC) $(R_FLAGS) counters-test.cxx matrix.cxx $(GTEST_FLAGS) -o $@

bench: benchmark
	./benchmark

benchmark: benchmark.cxx $(SOURCES)
	$(CC) $(R_FLAGS) benchmark.cxx $(BENCH_FLAGS) $(COMPILED_SOURCES) -o $@

clean:
	rm -f main

