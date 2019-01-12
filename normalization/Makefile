CPPFLAGS=$(CXXFLAGS) -O3 -Wall -Wextra -pedantic -std=c++11 -march=native

all: test

find: find.cpp
	$(CXX) $(CPPFLAGS) $< -o $@

unittest: unittest.cpp sse-test.c
	$(CXX) $(CPPFLAGS) $< -o $@

benchmark: benchmark.cpp sse-test.c ../benchmark.h
	$(CXX) $(CPPFLAGS) $< -o $@

parameters.txt: find
	./find > $@

sse-test.c: scripts/generate.py scripts/patterns.py parameters.txt
	python scripts/generate.py parameters.txt $@

test: sse-test.c
	$(CXX) -c -march=native $<
