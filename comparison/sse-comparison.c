#include "sse-comparison.h"

#include <stddef.h>
#include <stdlib.h>
#include <immintrin.h>

#define DEFINE_COMPARE(suffix, ROW_EXPR, COL_EXPR, USE_SHUFFLE2, SHUFFLE2)                      \
static uint64_t similarity_##suffix(SimilarityData* a, SimilarityData* b, uint64_t max) {       \
    uint64_t sim = 0;                                                                           \
    const __m128i shuffle2 = SHUFFLE2;                                                          \
    for (int i=0; i < 8; i++) {                                                                 \
        __m128i sums = _mm_setzero_si128(); /* 2 x 64 counters */                               \
        for (int j=0; j < 8; j++) {                                                             \
            const size_t ofs_a = 16 * (i * 4 + j);                                              \
            const size_t ofs_b = 16 * (ROW_EXPR) * 32 + (COL_EXPR);                             \
            const __m128i block1_r = _mm_loadu_si128((const __m128i*)(&a->avg_r[ofs_a]));       \
            const __m128i block1_g = _mm_loadu_si128((const __m128i*)(&a->avg_g[ofs_a]));       \
            const __m128i block1_b = _mm_loadu_si128((const __m128i*)(&a->avg_b[ofs_a]));       \
            __m128i block2_r = _mm_loadu_si128((const __m128i*)(&b->avg_r[ofs_b]));             \
            __m128i block2_g = _mm_loadu_si128((const __m128i*)(&b->avg_g[ofs_b]));             \
            __m128i block2_b = _mm_loadu_si128((const __m128i*)(&b->avg_b[ofs_b]));             \
            \
            if (USE_SHUFFLE2) {                                                                 \
                block2_r = _mm_shuffle_epi8(block2_r, shuffle2);                                \
                block2_g = _mm_shuffle_epi8(block2_g, shuffle2);                                \
                block2_b = _mm_shuffle_epi8(block2_b, shuffle2);                                \
            }                                                                                   \
            \
            const __m128i sr = _mm_sad_epu8(block1_r, block2_r);                                \
            const __m128i sg = _mm_sad_epu8(block1_g, block2_g);                                \
            const __m128i sb = _mm_sad_epu8(block1_b, block2_b);                                \
            \
            sums = _mm_add_epi64(sums, sr);                                                     \
            sums = _mm_add_epi64(sums, sg);                                                     \
            sums = _mm_add_epi64(sums, sb);                                                     \
        }                                                                                       \
        \
        sim += _mm_cvtsi128_si64(sums);                                                         \
        sim += _mm_cvtsi128_si64(_mm_bsrli_si128(sums, 8));                                     \
        if (sim > max) return MAX_DIFF;                                                         \
    }                                                                                           \
    return sim;                                                                                 \
}

// XXX: these lookups might be wrong, more unit testsing is needed;
//      but fortunatelly performance does not depend on pshufb values

#define NO_CHANGE _mm_setr_epi8( \
                  0,  1,  2,  3, \
                  4,  5,  6,  7, \
                  8,  9, 10, 11, \
                 12, 13, 14, 15)

#define REVERSE_HORIZ _mm_setr_epi8( \
                  3,  2,  1,  0, \
                  7,  6,  5,  4, \
                 11, 10,  9,  8, \
                 15, 14, 13, 12)

#define REVERSE_VERT _mm_setr_epi8( \
                 12, 13, 14, 15, \
                  8,  9, 10, 11, \
                  4,  5,  6,  7, \
                  0,  1,  2,  3)

#define REVERSE_HORIZ_VERT _mm_setr_epi8( \
                 15, 14, 13, 12, \
                 11, 10,  9,  8, \
                  7,  6,  5,  4, \
                  3,  2,  1,  0)

#define ROT_90 _mm_setr_epi8( \
                  0,  4,  8, 12, \
                  1,  5,  9, 13, \
                  2,  6, 10, 14, \
                  3,  7, 11, 15)

#define ROT_90_REVERSE_HORIZ _mm_setr_epi8( \
                 12,  8,  4,  0, \
                 13,  9,  5,  1, \
                 14, 10,  6,  2, \
                 15, 11,  7,  3)

#define ROT_90_REVERSE_VERT _mm_setr_epi8( \
                  3,  7, 11, 15, \
                  2,  6, 10, 14, \
                  1,  5,  9, 13, \
                  0,  4,  8, 12)

#define ROT_90_REVERSE_HORIZ_VERT _mm_setr_epi8( \
                 15, 11,  7,  3, \
                 14, 10,  6,  2, \
                 13,  9,  5,  1, \
                 12,  8,  4,  0)

DEFINE_COMPARE(0,      i,     j, 0, NO_CHANGE)
DEFINE_COMPARE(1,  7 - i,     j, 1, REVERSE_HORIZ)
DEFINE_COMPARE(2,      i, 7 - i, 1, REVERSE_VERT)
DEFINE_COMPARE(3,  7 - i, 7 - i, 0, REVERSE_HORIZ_VERT)
DEFINE_COMPARE(4,      j,     i, 0, ROT_90)
DEFINE_COMPARE(5,  7 - j,     i, 0, ROT_90_REVERSE_HORIZ)
DEFINE_COMPARE(6,      j, 7 - i, 0, ROT_90_REVERSE_VERT)
DEFINE_COMPARE(7,  7 - j, 7 - i, 0, ROT_90_REVERSE_HORIZ_VERT)

uint64_t similarity_sse(SimilarityData* a, SimilarityData* b) {
    
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
