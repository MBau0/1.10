#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

inline void print_b(int n) {
    for(int i = 0; i < (int) sizeof(int) * 8; ++i) {
        ( i % 8 == 0 && i != 0 ) ? printf(" ") : 0;
        ( n & 0x8000000 ) ? printf("1") : printf("0");
        n = n << 1;
    }
    printf("\n");
}

inline void print_b(char n) {
    for(int i = 0; i < 8; ++i) {
        ( n & 0x8000000 ) ? printf("1") : printf("0");
        n = n << 1;
    }
    printf("\n");
}

#endif