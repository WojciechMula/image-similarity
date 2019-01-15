#include "sse-blocking.h"

void sse_blocking_32x32(const uint8_t* src, uint8_t* dest) {
    uint8_t* block = dest;

    for (int block_y=0; block_y < 32/4; block_y++) {
        for (int block_x=0; block_x < 32/4; block_x++) {

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++) {
                    *block++ = src[(block_x * 4 + x) + (block_y * 4 + y) * 32];
                }
        }
    }
}
