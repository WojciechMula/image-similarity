all: test

find: find.cpp
	$(CXX) -O3 $< -o $@

parameters.txt: find
	./find > $@

sse.c: scripts/generate.py scripts/patterns.py parameters.txt
	python scripts/generate.py parameters.txt $@

test: sse.c
	$(CXX) -c -march=native $<
