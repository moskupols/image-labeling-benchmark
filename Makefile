CC=g++
COMMON_FLAGS = -Wall -Wextra --std=c++11 -fstack-protector -fsanitize=address -fsanitize=undefined
COMMON_FLAGS += -Isrc -I.

D_FLAGS=$(COMMON_FLAGS) -O0 -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread
BENCH_FLAGS=-lbenchmark -pthread
CIMG_FLAGS=-lX11 -pthread

SOURCES = src/* utils/*.hxx assets
COMPILED_SOURCES = src/*.cxx

BUILD_DIR ?= build

ULIMITED=ulimit -s 6100500 &&

BENCH_COUNTER_FILTER ?= ()
BENCH_MATRIX_FILTER ?= IntArrayMatrix

BENCH_FILTER ?= $(BENCH_COUNTER_FILTER).*$(BENCH_MATRIX_FILTER)
BENCH_RANDOM_FILTER ?= $(BENCH_COUNTER_FILTER).*RANDOM.*$(BENCH_MATRIX_FILTER)
BENCH_IMAGE_FILTER ?= $(BENCH_COUNTER_FILTER).*IMAGE.*$(BENCH_MATRIX_FILTER)

REPORT_FILE=report.csv

default: report

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
$(BUILD_DIR)/benchmark: benchmarks/* $(SOURCES) $(BUILD_DIR)/img.o
	$(CC) benchmarks/*.cxx $(COMPILED_SOURCES) $(BUILD_DIR)/img.o $(R_FLAGS) $(CIMG_FLAGS) $(BENCH_FLAGS) -o $@

$(BUILD_DIR)/img.o: utils/img.hxx utils/img.cxx
	$(CC) -c utils/img.cxx $(R_FLAGS) $(CIMG_FLAGS) -o $@

report: $(REPORT_FILE)

$(REPORT_FILE): utils/report.py benchmark
	$(ULIMITED) $(BUILD_DIR)/benchmark --benchmark_format=json --benchmark_filter='$(BENCH_FILTER)' | utils/report.py >$@

update-best:
	$(ULIMITED) $(BUILD_DIR)/benchmark --benchmark_format=json --benchmark_filter='$(BENCH_FILTER)' | utils/report.py --update-best >$(REPORT_FILE)
	git add best.json

clean:
	rm -f $(BUILD_DIR)/{img.o,benchmark,counters-test} $(REPORT_FILE)

