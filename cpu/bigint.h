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

    // Schoolbook subtraction
    static BigInt subtract(const BigInt& a, const BigInt& b) {
        // BigInt result to return
        BigInt result = BigInt(0);
        // If a has n limbs, and b has n limbs, then the result has at most n+m limbs
        int i = a.limbs.size();
        int j = b.limbs.size();

        // Before moving on to the algorithm, resize the results vector to have length matching 'a'
        for (int k = 1; k < i; k++)
        {
            result.limbs.push_back(0);
        }

        uint32_t borrow=0;
        uint32_t a_val;
        uint32_t b_val;
        uint64_t diff;
        uint32_t lower_diff;
        uint32_t upper_diff;
        for (int k = 0; k < i; k++)
        {
            a_val = a.limbs.at(k);
            if (k >= j) {
                // We have no more numbers to subtract
                b_val = 0;
            } else {
                b_val = b.limbs.at(k);
            }
            diff = (uint64_t)a_val - b_val - borrow; // Wraps around to huge number if negative
            lower_diff = diff & 0xFFFFFFFF;
            upper_diff = diff >> 32;
            if (upper_diff > 0) {
                // need to borrow because a_val was not big enough
                borrow = 1; // Now the next spot (an order of base magnitute greater) must account for an entire new unit of said greater magnitude
            } else {
                borrow = 0;
            }

            result.limbs[k] = lower_diff;
        }
        // Remove trailing zeros
        int idx = result.limbs.size() - 1;
        while (result.limbs.size() > 1 && result.limbs.at(idx) == 0)
        {
            result.limbs.erase(result.limbs.begin() + idx);
            idx--;
        }
        return result;
    }

    // Modulo - we don't actually need the mersenne number; only the power which it results from
    static BigInt mod_mersenne(const BigInt& a, int p) {
        BigInt result = BigInt(0);
        // Split x at bit position p
        int bin_num = p / 32;
        int split_point = p % 32;
        uint32_t x = a.limbs.at(bin_num);
        for (int i=0; i<bin_num; i++) {
            if (i==0) {
                result.limbs[i] = a.limbs.at(i);
            } else {
                result.limbs.push_back(a.limbs.at(i));
            }
        }
        // Now add the split point
        uint32_t lower_bits_split = x & ((1u << split_point)-1u);
        if (bin_num > 0) {
            result.limbs.push_back(lower_bits_split);
        } else {
            result.limbs[0] = lower_bits_split;
        }

        return result;
    }
};



#endif