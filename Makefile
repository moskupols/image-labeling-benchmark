CC=clang++
COMMON_FLAGS=--std=c++11
D_FLAGS=$(COMMON_FLAGS) -O0 -fstack-protector -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread

SOURCES=matrix.cxx matrix.h counter.h dfs_counter.h

run: release
	./main

main: release

debug: $(SOURCES)
	$(CC) $(D_FLAGS) -o main *.cxx

release: $(SOURCES)
	$(CC) $(R_FLAGS) -o main *.cxx

test: test-counters

test-counters: counters-test
	./counters-test

counters-test: counters-test.cxx matrix.cxx counter.h dfs_counter.h
	$(CC) $(D_FLAGS) $(GTEST_FLAGS) counters-test.cxx matrix.cxx -o $@

clean:
	rm -f main

