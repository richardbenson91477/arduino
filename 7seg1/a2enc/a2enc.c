#include <stdlib.h>
#include <stdio.h>

#define IN_N 1024

int main () {
    unsigned char bin[IN_N], *m;
    unsigned char e;

    while (fgets (bin, IN_N, stdin)) {
        e = 0;
        for (m = bin; *m; m++)
            switch (*m) {
                case 'A': e |= 1; break;
                case 'B': e |= 2; break;
                case 'C': e |= 4; break;
                case 'D': e |= 8; break;
                case 'E': e |= 16; break;
                case 'F': e |= 32; break;
                case 'G': e |= 64; break;
                case 'H': e |= 128; break;
            }
        printf ("0x%x\n", e);
    }

    return 0;
}

