CPPFLAGS=$(CXXFLAGS) -O3 -Wall -Wextra -pedantic -std=c++11 -march=native
FLAGS=$(CFLAGS) -O3 -march=native -std=c99

all: benchmark

benchmark: benchmark.cpp ../benchmark.h original.o specialisations.o
	$(CXX) $(CPPFLAGS) original.o specialisations.o $< -o $@

original.o: original.c original.h
	$(CC) $(FLAGS) $< -c -o $@

specialisations.o: specialisations.c specialisations.h
	$(CC) $(FLAGS) $< -c -o $@

