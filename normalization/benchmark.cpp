#include <cstdio>
#include "../benchmark.h"

#include "sse-test.c"


class Benchmark {

    static const size_t size = 32 * 32;

    uint8_t input[size];

public:
    void run() {

        char name[256];
        const size_t repeat = 10000;

        for (int i=0; i < 256; i++) {
            normalize_values_inplace_fn fun(algorithm1[i]);
            if (fun != NULL) {
                snprintf(name, sizeof(name), "algorithm1_%d", i);
                BEST_TIME(/**/, fun(0, input, size), name, repeat, size);
            }
        }

        for (int i=0; i < 256; i++) {
            normalize_values_inplace_fn fun(algorithm2[i]);
            if (fun != NULL) {
                snprintf(name, sizeof(name), "algorithm2_%d", i);
                BEST_TIME(/**/, fun(0, input, size), name, repeat, size);
            }
        }

        for (int i=0; i < 256; i++) {
            normalize_values_inplace_fn fun(algorithm3[i]);
            if (fun != NULL) {
                snprintf(name, sizeof(name), "algorithm3_%d", i);
                BEST_TIME(/**/, fun(0, input, size), name, repeat, size);
            }
        }

        for (int i=0; i < 256; i++) {
            normalize_values_inplace_fn fun(algorithm4[i]);
            if (fun != NULL) {
                snprintf(name, sizeof(name), "algorithm4_%d", i);
                BEST_TIME(/**/, fun(0, input, size), name, repeat, size);
            }
        }
    }

};


int main() {
    Benchmark benchmark;

    benchmark.run();
}
