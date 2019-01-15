CPPFLAGS=$(CXXFLAGS) -O3 -Wall -Wextra -pedantic -std=c++11 -march=native
FLAGS=$(CFLAGS) -O3 -march=native -std=c99

OBJS=original.o\
     specialisations.o\
     sse-blocking.o\
     sse-comparison.o

all: benchmark

benchmark: benchmark.cpp ../benchmark.h $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) $< -o $@

original.o: original.c original.h
	$(CC) $(FLAGS) $< -c -o $@

specialisations.o: specialisations.c specialisations.h
	$(CC) $(FLAGS) $< -c -o $@

sse-blocking.o: sse-blocking.c sse-blocking.h
	$(CC) $(FLAGS) $< -c -o $@

sse-comparison.o: sse-comparison.c sse-comparison.h
	$(CC) $(FLAGS) $< -c -o $@
