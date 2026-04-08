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

    // Schoolbook multiplication
    static BigInt multiply(const BigInt& a, const BigInt& b) {
        // BigInt result to return
        BigInt result = BigInt(0);
        // If a has n limbs, and b has n limbs, then the result has at most n+m limbs
        int i = a.limbs.size();
        int j = b.limbs.size();

        // Before moving on to the algorithm, resize the results vector
        for (int k=1; k<i+j; k++) {
            result.limbs.push_back(0);
        }

        uint64_t prod;
        uint32_t carry;
        uint32_t lower_32;
        uint64_t index_sum;
        for (int k=0; k<i; k++) {
            carry = 0;
            for (int l=0; l<j; l++) {
                prod = (uint64_t)a.limbs[k] * b.limbs[l] + carry;
                lower_32 = prod & 0xFFFFFFFF;
                carry = prod >> 32;
                index_sum = (uint64_t)result.limbs[k+l] + lower_32;
                carry += index_sum >> 32; // Carry from the index sum - safe because can't exceed 2^64 - 2^32
                result.limbs[k + l] = index_sum & 0xFFFFFFFF;
            }
            // Add any leftover cary
            result.limbs[k+j] += carry;
        }
        // Remove trailing zeros
        int idx = result.limbs.size()-1;
        while (result.limbs.size() > 1 && result.limbs.at(idx) == 0) {
            result.limbs.erase(result.limbs.begin() + idx);
            idx--;
        }
        return result;
    }
};



#endif