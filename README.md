# GPU-Accelerated Number Theoretic Transforms for Lucas-Lehmer Primality Testing

An HPC project implementing and benchmarking GPU-accelerated algorithms for the Lucas-Lehmer primality test, comparing Fast Fourier Transform (FFT) against Integer Number Theoretic Transform (NTT) performance on NVIDIA hardware.

## Overview

Mersenne primes are prime numbers of the form $M_p = 2^p - 1$. The Lucas-Lehmer test is the standard algorithm for verifying Mersenne prime candidates, requiring repeated squaring of massive integers. This project accelerates that process using CUDA and compares two approaches:

- **cuFFT-based multiplication**: Floating-point FFT convolution via NVIDIA's cuFFT library.
- **NTT-based multiplication**: Exact integer arithmetic using a Number Theoretic Transform modulo $2^{64} - 2^{32} + 1$.

## Deliverables

- Baseline CPU implementation of the Lucas-Lehmer test in C/C++
- CUDA kernel using cuFFT for large-integer multiplication
- CUDA kernel using integer NTT for large-integer multiplication
- Benchmarking suite for execution time and power consumption
- Final report comparing serial, FFT, and NTT performance

## Project Structure

```
├── cpu/          # Baseline CPU Lucas-Lehmer implementation
├── gpu/
│   ├── cufft/    # cuFFT-based CUDA kernel
│   └── ntt/      # NTT-based CUDA kernel
├── tests/        # Validation scripts against known Mersenne primes
├── benchmarks/   # Timing and power measurement scripts
└── docs/         # Final report and analysis
```

## Build Requirements

- C/C++ compiler with C++17 support
- NVIDIA CUDA Toolkit
- cuFFT library
- Access to NVIDIA GPU hardware

## References

1. GPUs Help Establish New Milestone in Mathematics (2024)
2. CUDALucas Project
3. PrMers: GPU-accelerated Mersenne Primality Testing
