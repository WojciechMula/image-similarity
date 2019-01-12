#include "sse.h"

#include <immintrin.h>
#include <string.h>
#include <stdio.h>
#include "sse-implementation.c"

static
void find_bounds_scalar(uint8_t* data, size_t n, uint8_t* low, uint8_t* range) {
    
    uint8_t max = data[0];
    uint8_t min = data[0];

    // 
    for (size_t i=0; i < n; i++) {
        if (data[i] < min)
            min = data[i];
        if (data[i] > max)
            max = data[i];
    }

    *low   = min;
    *range = max - min;
}

void normalize_sse(uint8_t* data, size_t n) {

    uint8_t min;
    uint8_t range;

    find_bounds_scalar(data, n, &min, &range);

    if (range == 0) {
        memset(data, 0, n);
        return;
    }

    if (range == 255) {
        return;
    }

    normalize_function_lookup[range](min, data, n);
}
