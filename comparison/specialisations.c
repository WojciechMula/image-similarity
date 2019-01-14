#include "specialisations.h"

#include <stddef.h>
#include <stdlib.h>

#define DEFINE_COMPARE(suffix, ROW_EXPR, COL_EXPR)                                                  \
    static uint64_t similarity_##suffix(SimilarityData* a, SimilarityData* b, uint64_t max) {       \
        int sim = 0;                                                                                \
        for (int i=0; i < 32; i++) {                                                                \
            for (int j=0; j < 32; j++) {                                                            \
                const size_t ofs_a = i * 32 + j;                                                    \
                const size_t ofs_b = (ROW_EXPR) * 32 + (COL_EXPR);                                  \
                sim += abs((int)a->avg_r[ofs_a] - (int)b->avg_r[ofs_b]);                            \
                sim += abs((int)a->avg_g[ofs_a] - (int)b->avg_g[ofs_b]);                            \
                sim += abs((int)a->avg_b[ofs_a] - (int)b->avg_b[ofs_b]);                            \
            }                                                                                       \
            if (sim > max) return MAX_DIFF;                                                                                \
        }                                                                                           \
        return sim;                                                                                 \
    }

DEFINE_COMPARE(0,       i,      j)
DEFINE_COMPARE(1,  31 - i,      j)
DEFINE_COMPARE(2,       i, 31 - i)
DEFINE_COMPARE(3,  31 - i, 31 - i)
DEFINE_COMPARE(4,       j,      i)
DEFINE_COMPARE(5,  31 - j,      i)
DEFINE_COMPARE(6,       j, 31 - i)
DEFINE_COMPARE(7,  31 - j, 31 - i)



uint64_t similarity_specialisations(SimilarityData* a, SimilarityData* b) {
    
    uint64_t min = MAX_DIFF + 1;
    uint64_t ret;

    ret = similarity_0(a, b, min);
    if (ret < min) min = ret;

    ret = similarity_1(a, b, min);
    if (ret < min) min = ret;

    ret = similarity_2(a, b, min);
    if (ret < min) min = ret;

    ret = similarity_3(a, b, min);
    if (ret < min) min = ret;

    ret = similarity_4(a, b, min);
    if (ret < min) min = ret;

    ret = similarity_5(a, b, min);
    if (ret < min) min = ret;

    ret = similarity_6(a, b, min);
    if (ret < min) min = ret;

    ret = similarity_7(a, b, min);
    if (ret < min) min = ret;

    return min;
}
