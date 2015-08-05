CC=g++
COMMON_FLAGS=--std=c++11 -fstack-protector -fsanitize=address -fsanitize=undefined

D_FLAGS=$(COMMON_FLAGS) -O0 -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread
BENCH_FLAGS=-lbenchmark -pthread

SOURCES=matrix.cxx matrix.h int_array.cxx int_array.h counter.h dfs_counter.h
COMPILED_SOURCES=matrix.cxx int_array.cxx

run: main
	./main

main: debug

debug: $(SOURCES)
	$(CC) $(D_FLAGS) -o main main.cxx $(COMPILED_SOURCES)

release: $(SOURCES) main.cxx
	$(CC) $(R_FLAGS) -o main main.cxx $(COMPILED_SOURCES)

test: test-counters

test-counters: counters-test
	ulimit -s 100500
	./counters-test

counters-test: counters-test.cxx $(SOURCES)
	$(CC) $(R_FLAGS) counters-test.cxx $(COMPILED_SOURCES) $(GTEST_FLAGS) -o $@

bench: benchmark
	./benchmark

benchmark: benchmark.cxx $(SOURCES)
	$(CC) $(R_FLAGS) benchmark.cxx $(BENCH_FLAGS) $(COMPILED_SOURCES) -o $@

clean:
	rm -f main

