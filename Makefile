CC=clang++
COMMON_FLAGS=--std=c++11
D_FLAGS=$(COMMON_FLAGS) -O0 -fstack-protector -ggdb
R_FLAGS=$(COMMON_FLAGS) -O3

run: release
	./main

main: release

debug: main.cxx matrix.cxx
	$(CC) $(D_FLAGS) -o main *.cxx

release: main.cxx matrix.cxx
	$(CC) $(R_FLAGS) -o main *.cxx

clean:
	rm -f main

