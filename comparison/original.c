#include "common.h"
#include "original.h"

#include <stdlib.h>
#include <stddef.h>


static uint64_t similarity_generic_impl(SimilarityData* a, SimilarityData* b, int orientation, uint64_t max);

uint64_t similarity_generic(SimilarityData* a, SimilarityData* b) {
    
    uint64_t min = MAX_DIFF + 1;

    for (int orientation = 0; orientation < 8; orientation++) {
        uint64_t ret = similarity_generic_impl(a, b, orientation, min);
        if (ret < min) {
            min = ret;
        }
    }

    return min;
}

static uint64_t similarity_generic_impl(SimilarityData* a, SimilarityData* b, int orientation, uint64_t max) {

    int i;
    int j;
    int x;
    int y;
    int* row;
    int* col;

    if (orientation & 0x1) {
        row = &y;
        col = &x;
    } else {
        row = &x;
        col = &y;
    }

    int sim = 0;
    for (int i=0; i < 32; i++) {
        if (orientation & 0x2)
            *row = 31 - i;
        else
            *row = i;

        for (int j=0; j < 32; j++) {
            if (orientation & 0x4)
                *col = 31 - j;
            else
                *col = j;

            const size_t ofs_a = i * 32 + j;
            const size_t ofs_b = (*row) * 32 + (*col);

            sim += abs((int)a->avg_r[ofs_a] - (int)b->avg_r[ofs_b]);
            sim += abs((int)a->avg_g[ofs_a] - (int)b->avg_g[ofs_b]);
            sim += abs((int)a->avg_b[ofs_a] - (int)b->avg_b[ofs_b]);
        }
        if (sim > max) return MAX_DIFF;
    }

    return sim;
}
