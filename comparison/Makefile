CPPFLAGS=$(CXXFLAGS) -O3 -Wall -Wextra -pedantic -std=c++11 -march=native
FLAGS=$(CFLAGS) -O3 -march=native -std=c99

all: benchmark

benchmark: benchmark.cpp ../benchmark.h original.o
	$(CXX) $(CPPFLAGS) original.o $< -o $@

original.o: original.c original.h
	$(CC) $(FLAGS) $< -c -o $@