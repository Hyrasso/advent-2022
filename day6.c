#include <stdio.h>

#define N 5000

#define MLEN 14

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day6.txt", "r");

    char signal[N];

    fgets(signal, N, file);

    size_t i = MLEN - 1;
    while(signal[i]) {
        unsigned int mask = 0;
        for (size_t j=i-(MLEN-1);j<=i;j++) {
            unsigned int new_bit = 1 << (signal[j] - 'a');
            mask |= new_bit;
        }
        if (__builtin_popcount(mask) == MLEN) {
            break;
        }
        i++;
    }
    printf("i: %zu\n", i+1);

    return 0;
}