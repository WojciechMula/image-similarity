#pragma once

#include <stdint.h>

/*
    Transform an image 32 x 32 saved in continous region
    into series of subimages 4 x 4 pixels; such a subimage
    is fit in an SSE register.
*/
void sse_blocking_32x32(const uint8_t* linear, uint8_t* dest);

