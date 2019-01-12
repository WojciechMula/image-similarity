DECLARATION="void %(name)s(uint8_t* data, size_t size);"

ALGORITHM1_PATTERN_GENERIC="""
void %(name)s(uint8_t* data, size_t size) {

    const __m128i scale = _mm_set1_epi16(%(scale)s);

    for (size_t i=0; i < size; i += 16) {
        const __m128i input = _mm_loadu_si128((const __m128i*)(data + i));

        // convert 8-bit array into two 16-bit arrays
        const __m128i in0 = _mm_cvtepu8_epi16(input);
        const __m128i in1 = _mm_cvtepu8_epi16(_mm_bsrli_si128(input, 8));

        // scale
        const __m128i a0 = _mm_mullo_epi16(in0, scale);
        const __m128i a1 = _mm_mullo_epi16(in1, scale);

        // shift
        const __m128i b0 = _mm_srli_epi16(a0, %(shift)s);
        const __m128i b1 = _mm_srli_epi16(a1, %(shift)s);

        // convert back to array of bytes
        const __m128i res = _mm_packus_epi16(b0, b1);
        _mm_store_si128((__m128i*)(data + i), res);
    }
}
"""

ALGORITHM1_PATTERN_NO_SHIFT="""
void %(name)s(uint8_t* data, size_t size) {

    const __m128i scale = _mm_set1_epi16(%(scale)s);

    for (size_t i=0; i < size; i += 16) {
        const __m128i input = _mm_loadu_si128((const __m128i*)(data + i));

        // convert 8-bit array into two 16-bit arrays
        const __m128i in0 = _mm_cvtepu8_epi16(input);
        const __m128i in1 = _mm_cvtepu8_epi16(_mm_bsrli_si128(input, 8));

        // scale
        const __m128i a0 = _mm_mullo_epi16(in0, scale);
        const __m128i a1 = _mm_mullo_epi16(in1, scale);

        // convert back to array of bytes
        const __m128i res = _mm_packus_epi16(a0, a1);
        _mm_store_si128((__m128i*)(data + i), res);
    }
}
"""

ALGORITHM2_PATTERN_GENERIC="""
void %(name)s(uint8_t* data, size_t size) {

    const __m128i scale = _mm_set1_epi16(%(scale)s);

    for (size_t i=0; i < size; i += 16) {
        const __m128i input = _mm_loadu_si128((const __m128i*)(data + i));

        // convert 8-bit values to 16-bit multiplied by 256
        // X[i] = (uint16_t)x[i] * 256
        const __m128i in0 = _mm_unpacklo_epi8(_mm_setzero_si128(), input);
        const __m128i in1 = _mm_unpackhi_epi8(_mm_setzero_si128(), input);

        // scale
        const __m128i a0 = _mm_mulhi_epu16(in0, scale);
        const __m128i a1 = _mm_mulhi_epu16(in1, scale);

        // shift
        const __m128i b0 = _mm_srli_epi16(a0, %(shift)s);
        const __m128i b1 = _mm_srli_epi16(a1, %(shift)s);

        // convert back to array of bytes
        const __m128i res = _mm_packus_epi16(b0, b1);
        _mm_store_si128((__m128i*)(data + i), res);
    }
}
"""

ALGORITHM2_PATTERN_NO_SHIFT="""
void %(name)s(uint8_t* data, size_t size) {

    const __m128i scale = _mm_set1_epi16(%(scale)s);

    for (size_t i=0; i < size; i += 16) {
        const __m128i input = _mm_loadu_si128((const __m128i*)(data + i));

        // convert 8-bit values to 16-bit multiplied by 256
        // X[i] = (uint16_t)x[i] * 256
        const __m128i in0 = _mm_unpacklo_epi8(_mm_setzero_si128(), input);
        const __m128i in1 = _mm_unpackhi_epi8(_mm_setzero_si128(), input);

        // scale
        const __m128i a0 = _mm_mulhi_epu16(in0, scale);
        const __m128i a1 = _mm_mulhi_epu16(in1, scale);

        // convert back to array of bytes
        const __m128i res = _mm_packus_epi16(a0, a1);
        _mm_store_si128((__m128i*)(data + i), res);
    }
}
"""

PSHUFB_ALGORITHM="""
void %(name)s(uint8_t* data, size_t size) {

    const __m128i lookup = _mm_setr_epi8(%(lookup_values)s);

    for (size_t i=0; i < size; i += 16) {
        const __m128i input = _mm_loadu_si128((const __m128i*)(data + i));

        const __m128i res = _mm_shuffle_epi8(lookup, input);
        _mm_store_si128((__m128i*)(data + i), res);
    }
}
"""

SCALAR_ALGORITHM="""
void %(name)s(uint8_t* data, size_t size) {

    const double scale = 255.0 / %(value)s;
    for (size_t i=0; i < size; i++)
        data[i] = scale * data[i];
}
"""

