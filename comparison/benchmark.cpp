#include <cstdio>
#include "../benchmark.h"

#include "common.h"

extern "C" {

    #include "original.h"
}


class Benchmark {

    SimilarityData a;
    SimilarityData b;

public:
    void run() {

        const size_t repeat = 10000;

        BEST_TIME(/**/, similarity_generic(&a, &b), "generic", repeat, 1);
    }

};


int main() {
    Benchmark benchmark;

    benchmark.run();
}
