CC=clang++
CXXFLAGS=--std=c++11 -fstack-protector -O0 -ggdb

run: main
	./main

main: main.cxx matrix.cxx
	$(CC) $(CXXFLAGS) -o $@ *.cxx

debug: main
release: main

clean:
	rm -f main

