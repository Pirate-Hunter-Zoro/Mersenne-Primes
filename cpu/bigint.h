#ifndef BIGINT_H
#define BIGINT_H

#include <vector>
#include <cstdint>
#include <algorithm>

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

    static BigInt add(const BigInt& a, const BigInt& b) {
        // BigInt result to return
        BigInt result = BigInt(0);
        // If a has n limbs, and b has n limbs, then the result has at most n+m limbs
        int i = a.limbs.size();
        int j = b.limbs.size();

        // Before moving on to the algorithm, resize the results vector to have length matching 'a'
        for (int k = 1; k < std::max(i, j)+1; k++)
        {
            result.limbs.push_back(0);
        }

        // Perform addition
        uint32_t carry = 0;
        uint32_t a_val;
        uint32_t b_val;
        uint64_t sum;
        uint32_t lower_sum;
        for (int k=0; k<std::max(i,j); k++) {
            if (k < i) {
                a_val = a.limbs.at(k);
            } else {
                a_val = 0;
            }
            if (k < j) {
                b_val = b.limbs.at(k);
            } else {
                b_val = 0;
            }
            // Compute sum and carry
            sum = (uint64_t)a_val + b_val + carry;
            lower_sum = sum & 0xFFFFFFFF;
            result.limbs[k] = lower_sum;
            carry = sum >> 32;
        }
        if (carry > 0) {
            result.limbs[std::max(i,j)] = carry;
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
        BigInt result = a;

        // Split at bit position p
        int bin_num = p / 32;
        int split_point = p % 32;
        bool equal;

        while ((result.limbs.size() > bin_num+1) || (result.limbs.size() == bin_num+1 && (result.limbs.at(result.limbs.size()-1) >= (1u << split_point)))) { 
            // While result does not fit within p bits
            BigInt low_result = BigInt(0);
            uint32_t x = result.limbs.at(bin_num);
            for (int i=0; i<bin_num; i++) {
                if (i==0) {
                    low_result.limbs[i] = result.limbs.at(i);
                } else {
                    low_result.limbs.push_back(result.limbs.at(i));
                }
            }
            // Now add the split point
            uint32_t lower_bits_split = x & ((1u << split_point)-1u);
            if (bin_num > 0) {
                low_result.limbs.push_back(lower_bits_split);
            } else {
                low_result.limbs[0] = lower_bits_split;
            }

            // Now we need the higher bits in the split 
            uint32_t higher_bits_split = x >> split_point;
            BigInt high_result = BigInt(higher_bits_split);
            for (int i=bin_num+1; i<result.limbs.size(); i++) {
                // Shift in the lower 32-split_point bits from this bin into the previous new bin
                high_result.limbs[high_result.limbs.size()-1] += result.limbs.at(i) << (32 - split_point);
                // Shift in the upper split_point bits from this bin into the first split_point bits of the current new bin
                high_result.limbs.push_back(result.limbs.at(i) >> split_point);
            }

            result = add(low_result, high_result);
        }

        // Edge case - check for equality in which case return 0
        if (result.limbs.size() == bin_num + 1)
        {
            equal = 1;
            for (int i = 0; i <= bin_num; i++)
            {
                if ((i < bin_num) && (result.limbs.at(i) != 0xFFFFFFFF))
                {
                    equal = 0;
                    break;
                }
                else if ((i == bin_num) && (result.limbs.at(i) != (1u << split_point) - 1))
                {
                    equal = 0;
                    break;
                }
            }
            if (equal)
            {
                return BigInt(0);
            }
        }

        return result;
    }
};



#endif