#include <cstdint>
#include <cassert>
#include <cstdio>

#include "../ansi.cpp"
#include "scalar.c"


class UnitTest {

    static const size_t size = 32 * 32;

    uint8_t input_reference[size];
    uint8_t output_reference[size];
    uint8_t input[size];

    uint8_t value;

    struct Failed {};

public:
    UnitTest() : value(0) {}

    bool run() {
        prepare_input();
        try {
            for (value=1; value <= 254; value++) {
                printf("Testing range [0 .. %d]", value); fflush(stdout);
                test();
            }

            return true;
        } catch (Failed&) {
            putchar('\n');
            dump(output_reference, value + 1);
            dump(input, value + 1);
            printf("%s\n", ansi::seq("FAILED", ansi::RED).c_str());
            return false;
        }
    }

private:
    void test() {
        prepare_input();
        memcpy(output_reference, input_reference, size);
        normalize_scalar(output_reference, size);

        if (algorithm1[value] != nullptr) {
            printf(" algorithm1"); fflush(stdout);
            memcpy(input, input_reference, size);
            algorithm1[value](input, size);

            if (memcmp(input, output_reference, size) != 0) {
                throw Failed{};
            }
        }

        if (algorithm2[value] != nullptr) {
            printf(" algorithm2"); fflush(stdout);
            memcpy(input, input_reference, size);
            algorithm2[value](input, size);

            if (memcmp(input, output_reference, size) != 0) {
                throw Failed{};
            }
        }

        if (algorithm3[value] != nullptr) {
            printf(" algorithm3"); fflush(stdout);
            memcpy(input, input_reference, size);
            algorithm3[value](input, size);

            if (memcmp(input, output_reference, size) != 0) {
                throw Failed{};
            }
        }

        if (algorithm4[value] != nullptr) {
            printf(" algorithm4"); fflush(stdout);
            memcpy(input, input_reference, size);
            algorithm4[value](input, size);

            if (memcmp(input, output_reference, size) != 0) {
                throw Failed{};
            }
        }

        putchar('\n');
    }

    void prepare_input() {
        for (size_t i=0; i < size; i++)
            input_reference[i] = i % (value + 1); // [0 .. value]
    }

    void dump(const uint8_t* data, size_t size) {
        printf("{%d", data[0]);
        for (size_t i=1; i < size; i++) {
            printf(", %d", data[i]);
        }
        printf("}\n");
    }
};


int main() {
    
    UnitTest test;

    test.run();
}
