#ifndef BIGINT_H
#define BIGINT_H

#include <vector>
#include <cstdint>

struct BigInt {
    std::vector<uint32_t> limbs;

    BigInt(uint64_t value) {
        // Grab the upper 32 bits
        uint32_t upper_32 = value >> 32;
        uint32_t lower_32 = value & 0xFFFFFFFF;
        limbs.push_back(lower_32);
        if (upper_32 > 0) {
            limbs.push_back(upper_32);
        }
    }
};

#endif