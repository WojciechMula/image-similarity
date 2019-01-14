#pragma once

#include <stdint.h>

typedef struct SimilarityData {
    uint8_t avg_r[32 * 32];
    uint8_t avg_g[32 * 32];
    uint8_t avg_b[32 * 32];
} SimilarityData;

#define MAX_DIFF (32 * 32 * 255 * 3) // dim * max_value * num_channels

