#pragma once

#include "common.h"

// There's assumption that SimilarityData has data which
// was already by sse_blocking_32x32() from sse-blocking.h
uint64_t similarity_sse(SimilarityData* a, SimilarityData* b);

