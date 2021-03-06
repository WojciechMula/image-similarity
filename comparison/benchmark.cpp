#include <cstdio>
#include "../benchmark.h"

#include "common.h"

extern "C" {

    #include "original.h"
    #include "specialisations.h"
    #include "sse-comparison.h"
}


class Benchmark {

    SimilarityData a;
    SimilarityData b;

public:
    void run() {

        const size_t repeat = 10000;

        BEST_TIME(/**/, similarity_generic(&a, &b),         "generic",          repeat, 1);
        BEST_TIME(/**/, similarity_specialisations(&a, &b), "specialisations",  repeat, 1);
        BEST_TIME(/**/, similarity_sse(&a, &b),             "SSE",              repeat, 1);
    }

};


int main() {
    Benchmark benchmark;

    benchmark.run();
}
