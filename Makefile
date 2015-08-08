CC=g++
COMMON_FLAGS = -Wall -Wextra --std=c++11 -fstack-protector -fsanitize=address -fsanitize=undefined
COMMON_FLAGS += -Icounters -I.

D_FLAGS=$(COMMON_FLAGS) -O0 -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread
BENCH_FLAGS=-lbenchmark -pthread
CIMG_FLAGS=-lX11 -pthread

SOURCES = counters/* utils/testgen.h
COMPILED_SOURCES = counters/*.cxx

BUILD_DIR ?= build

ULIMITED=ulimit -s 6100500 &&

BENCH_FILTER=(Dsu|Stack|<IntArray)
BENCH_RANDOM_FILTER=$(BENCH_FILTER).*RANDOM
BENCH_IMAGE_FILTER=$(BENCH_FILTER).*IMAGE

REPORT_FILE=report.csv

run: main
	$(BUILD_DIR)/main

main: debug

debug: $(SOURCES)
	$(CC) $(D_FLAGS) -o $(BUILD_DIR)/main main.cxx $(COMPILED_SOURCES)

release: $(SOURCES) main.cxx
	$(CC) $(R_FLAGS) -o $(BUILD_DIR)/main main.cxx $(COMPILED_SOURCES)

test: counters-test
	$(ULIMITED) $(BUILD_DIR)/counters-test

counters-test: $(BUILD_DIR)/counters-test
$(BUILD_DIR)/counters-test: counters-test.cxx $(SOURCES)
	$(CC) $(R_FLAGS) counters-test.cxx $(COMPILED_SOURCES) $(GTEST_FLAGS) -o $@

bench: benchmark
	$(ULIMITED) $(BUILD_DIR)/benchmark --benchmark_filter='$(BENCH_FILTER)'

bench-random: benchmark
	$(ULIMITED) $(BUILD_DIR)/benchmark --benchmark_filter='$(BENCH_RANDOM_FILTER)'

bench-image: benchmark
	$(ULIMITED) $(BUILD_DIR)/benchmark --benchmark_filter='$(BENCH_IMAGE_FILTER)'

benchmark: $(BUILD_DIR)/benchmark
$(BUILD_DIR)/benchmark: benchmarks/* $(SOURCES)
	$(CC) benchmarks/*.cxx $(COMPILED_SOURCES) $(R_FLAGS) $(CIMG_FLAGS) $(BENCH_FLAGS) -o $@

report: $(REPORT_FILE)

$(REPORT_FILE): utils/report.py benchmark
	$(ULIMITED) $(BUILD_DIR)/benchmark --benchmark_format=json --benchmark_filter='$(BENCH_FILTER)' | utils/report.py >$@

clean:
	rm -f $(BUILD_DIR)/{main,benchmark,counters-test} $(REPORT_FILE)

