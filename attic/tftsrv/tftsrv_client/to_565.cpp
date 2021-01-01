
#include <cstdio>
#include <cstdint>

uint16_t to_rgb565 (uint16_t r, uint16_t g, uint16_t b) {
    return 
        ((r >> 3) << 11) +\
        ((g >> 2) << 5) +\
         (b >> 3);
}

int main (int argc, char **argv) {
    FILE *f_in;
    int r, g, b;
    uint16_t rgb565;

    if (argc != 2) {
        fprintf (stderr, "usage: to565 input_file.rgb \n");
        return -1;
    }

    f_in = fopen (argv[1], "rb");
    if (! f_in) { 
        fprintf (stderr, "ERROR: fopen rb: %s\n", argv[1]);
        return -1;
    }
 
    while ((r = fgetc (f_in)) != EOF) {
        g = fgetc (f_in);
        b = fgetc (f_in);
        rgb565 = to_rgb565 (r, g, b);
        printf ("%c%c", rgb565 & 255, rgb565 >> 8);
    }

    fclose (f_in);
    return 0;
}

