CPPFLAGS=$(CXXFLAGS) -O3 -Wall -Wextra -pedantic -std=c++11 -march=native
FLAGS=$(CFLAGS) -O3 -march=native -std=c99

all: test

find: find.cpp
	$(CXX) $(CPPFLAGS) $< -o $@

unittest: unittest.cpp sse-test.c
	$(CXX) $(CPPFLAGS) $< -o $@

benchmark: benchmark.cpp sse-test.c ../benchmark.h
	$(CXX) $(CPPFLAGS) $< -o $@

speed: speed.cpp scalar.o ../benchmark.h
	$(CXX) $(CPPFLAGS) $< scalar.o -o $@

scalar.o: scalar.c
	$(CC) $(FLAGS) $< -c -o $@

parameters.txt: find
	./find > $@

sse-test.c: scripts/generate-test.py scripts/generate_base.py scripts/patterns.py parameters.txt
	python scripts/generate-test.py parameters.txt $@

sse-implementation.c: scripts/generate.py scripts/generate_base.py scripts/patterns.py parameters.txt
	python scripts/generate.py parameters.txt $@

test: sse-test.c
	$(CXX) -c -march=native $<
