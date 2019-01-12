#include <cstdio>
#include <cstring>

#include "../benchmark.h"

extern "C" {
#   include "scalar.h"
#   include "sse.h"
}

class Benchmark {

    static const size_t size = 32 * 32;
    static const size_t count = 256;

    uint8_t input[count][size];

public:
    void run() {
        const size_t repeat = 100;

        BEST_TIME(/**/, test_empty(),  "empty",  repeat, count * size);
        BEST_TIME(/**/, test_scalar(), "scalar", repeat, count * size);
        BEST_TIME(/**/, test_SSE(),    "SSE",    repeat, count * size);
    }

private:
    void prepare_input() {
        for (int i=0; i < 256; i++) {
            memset(&input[i], i, size);
        }
    }

    void test_empty() {
        prepare_input();
        for (int i=0; i < 256; i++) {
            // deliberately nop
        }
    }

    void test_scalar() {
        prepare_input();
        for (int i=0; i < 256; i++) {
            normalize_scalar(&input[i][0], size);
        }
    }

    void test_SSE() {
        prepare_input();
        for (int i=0; i < 256; i++) {
            normalize_sse(&input[i][0], size);
        }
    }

};


int main() {
    Benchmark benchmark;

    benchmark.run();
}
