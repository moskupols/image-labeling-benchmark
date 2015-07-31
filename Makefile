CC=clang++
COMMON_FLAGS=--std=c++11
D_FLAGS=$(COMMON_FLAGS) -O0 -fstack-protector -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread

SOURCES=matrix.cxx matrix.h counter.h dfs_counter.h
COMPILED_SOURCES=matrix.cxx main.cxx

run: main
	./main

main: debug

debug: $(SOURCES)
	$(CC) $(D_FLAGS) -o main $(COMPILED_SOURCES)

release: $(SOURCES) main.cxx
	$(CC) $(R_FLAGS) -o main $(COMPILED_SOURCES)

test: test-counters

test-counters: counters-test
	./counters-test

counters-test: counters-test.cxx $(SOURCES)
	$(CC) $(D_FLAGS) $(GTEST_FLAGS) counters-test.cxx matrix.cxx -o $@

clean:
	rm -f main

