#include <vector>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
    // Lucas-Lehmer test
    if (argc < 2) {
        printf("Please specify which Mersenne number you want to check for primeness...\n");
        return 1;
    }
    int p=std::atoi(argv[1]);
    unsigned long long m_p=(1<<p) - 1;

    unsigned long long s = 4;
    for (int i=0; i<p-2; i++) {
        s = s * s - 2;
        s = s % m_p;
    }
    printf("Mersenne number %d (%d) is ", p, (1<<p) - 1);
    if (s == 0 || m_p==3) {
        // Prime
        printf("prime!\n"); 
    } else {
        printf("not prime!\n");
    }

    return 0;
}