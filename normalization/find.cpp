#include <cstdint>
#include <map>
#include <vector>
#include <cstdint>

class Finder {
    using Vector = std::vector<uint8_t>;
    using Vector32 = std::vector<uint32_t>;

    std::map<int, Vector> precalculated;

public:
    Finder() {
        fill_precalculated();
    }

    void run() {

        Vector32 intermediate1(256);
        Vector32 intermediate2(256);
        for (uint32_t scale = 1; scale < 65536; scale++) {
            for (int i=0; i < 256; i++) {
                intermediate1[i] = i * scale;
                intermediate2[i] = (i * scale) << 8;
            }

            for (int shift = 0; shift < 16; shift++) {

                auto algorithm1 = [shift](uint32_t x) -> uint32_t {
                    x = (x & 0xffff) >> shift;
                    return x;
                };

                auto algorithm2 = [shift](uint32_t x) -> uint32_t {
                    return x >> (16 + shift);
                };

                for (const auto& item: precalculated) {
                    const Vector& reference = item.second;

                    if (match(reference, intermediate1, algorithm1)) {
                        printf("algorithm1 value = %d scale = %d shift = %d\n", item.first, scale, shift);
                    }

                    if (match(reference, intermediate2, algorithm2)) {
                        printf("algorithm2 value = %d scale = %d shift = %d\n", item.first, scale, shift);
                    }
                }
            }
        }
    }

private:
    void fill_precalculated() {
        for (int value=16; value < 255; value++) {
            double scale = 255.0 / value;
            precalculated[value].resize(value + 1);

            for (int i=0; i <= value; i++)
                precalculated[value][i] = scale * i;
        }
    }

    template <typename FUN>
    bool match(const Vector& reference, const Vector32& v, FUN f) const {
        if (reference.size() > v.size())
            return false;

        for (size_t i=0; i < reference.size(); i++) {
            if (uint32_t(reference[i]) != f(v[i]))
                return false;
        }

        return true;
    }
};


int main() {
    Finder f;
    f.run();
}
