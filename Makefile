CC=g++
COMMON_FLAGS = -Wall -Wextra --std=c++11
COMMON_FLAGS += -Isrc -I.

D_FLAGS=$(COMMON_FLAGS) -O0 -ggdb -fstack-protector -fsanitize=address -fsanitize=undefined
R_FLAGS=$(COMMON_FLAGS) -O3

GTEST_FLAGS=-lgtest_main -lgtest -pthread
BENCH_FLAGS=-lbenchmark -pthread
CIMG_FLAGS=-lX11 -pthread

SOURCES = src/* utils/*.hxx assets
COMPILED_SOURCES = src/*.cxx

SIZES = $(shell grep -P '^\w' benchmarks/bench-random.cxx | grep -oP '\d+, \d+' | uniq | sed 's/, /x/')
SIZE_CSV_DIR ?= bench-out
SIZE_CSVS = $(patsubst %,$(SIZE_CSV_DIR)/%.csv,$(SIZES))

PIC_DIR ?= paper/pics
SIZE_PIC = $(patsubst %.csv,$(PIC_DIR)/%.eps,$(notdir $(SIZE_CSVS)))

TABLE_DIR ?= paper/tables
SIZE_TEX = $(patsubst %.csv,$(TABLE_DIR)/%.tex,$(notdir $(SIZE_CSVS)))

BUILD_DIR ?= build

ULIMITED=ulimit -s 6100500 &&

BENCH_COUNTER_FILTER ?= (.*(Dfs|Dsu).*)
BENCH_MATRIX_FILTER ?= ArrayMatrix

# BENCH_FILTER ?= $(BENCH_COUNTER_FILTER).*$(BENCH_MATRIX_FILTER)
BENCH_RANDOM_FILTER ?= $(BENCH_COUNTER_FILTER).*RANDOM.*$(BENCH_MATRIX_FILTER)
BENCH_IMAGE_FILTER ?= $(BENCH_COUNTER_FILTER).*IMAGE.*$(BENCH_MATRIX_FILTER)
BENCH_FILTER ?= $(BENCH_RANDOM_FILTER)

BENCH_RUN_FLAGS ?= --benchmark_min_time=15.0

REPORT_FILE=report.csv

default: report

test: counters-test
	$(ULIMITED) $(BUILD_DIR)/counters-test

counters-test: $(BUILD_DIR)/counters-test
$(BUILD_DIR)/counters-test: counters-test.cxx $(SOURCES)
	$(CC) $(D_FLAGS) counters-test.cxx $(COMPILED_SOURCES) $(GTEST_FLAGS) -o $@

benchmark: $(BUILD_DIR)/benchmark
$(BUILD_DIR)/benchmark: benchmarks/* $(SOURCES) $(BUILD_DIR)/img.o
	$(CC) benchmarks/*.cxx $(COMPILED_SOURCES) $(BUILD_DIR)/img.o $(R_FLAGS) $(CIMG_FLAGS) $(BENCH_FLAGS) -o $@

$(BUILD_DIR)/img.o: utils/img.hxx utils/img.cxx
	$(CC) -c utils/img.cxx $(R_FLAGS) $(CIMG_FLAGS) -o $@

report: $(REPORT_FILE)

$(REPORT_FILE): bench.json utils/report.py
	utils/report.py <$< >$@

split-sizes: $(SIZE_CSVS)
$(SIZE_CSVS): bench.json utils/report.py
	mkdir -p $(SIZE_CSV_DIR)
	utils/report.py --split-by-sizes <$<

latex-figures: $(SIZE_PIC) $(SIZE_TEX)
$(SIZE_PIC): split-sizes utils/csv-to-latex.py
	mkdir -p $(PIC_DIR)
	utils/csv-to-latex.py --plot $@ <$(patsubst %.eps,$(SIZE_CSV_DIR)/%.csv,$(notdir $@)) >/dev/null
$(SIZE_TEX): split-sizes utils/csv-to-latex.py
	mkdir -p $(TABLE_DIR)
	utils/csv-to-latex.py <$(patsubst %.tex,$(SIZE_CSV_DIR)/%.csv,$(notdir $@)) >$@

paper: latex-figures paper/paper.tex
	cd paper && latexmk -pdf paper

bench.json: $(BUILD_DIR)/benchmark
	$(ULIMITED) time $(BUILD_DIR)/benchmark $(BENCH_RUN_FLAGS) --benchmark_format=json --benchmark_filter='$(BENCH_FILTER)' > $@

update-best: bench.json utils/report.py
	utils/report.py --update-best <$< >/dev/null

clean:
	rm -f $(BUILD_DIR)/{img.o,benchmark,counters-test} $(REPORT_FILE)

