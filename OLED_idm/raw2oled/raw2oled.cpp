// converts gimp exported "raw data" monochrome files to SSD1306 format
// 2018 richardenson91477@gmail.com

#include <cstdlib>
#include <cstdio> 
#include <cstdint>

int main (int argc, char *argv[]) {
    uint8_t *_b;
    uint8_t _b_raw [1024];
    uint8_t _b_in [8192];

    if (argc < 2) {
        fprintf (stderr, "usage: %s [filename]\n", argv[0]);
        exit (-1);
    }
    FILE *_f = fopen (argv[1], "rb");
    if (! _f) {
        fprintf (stderr, "ERROR: can't open %s\n", argv[1]);
        exit (-2);
    }
    if (fread (_b_in, 1, 8192, _f) != 8192) {
        fprintf (stderr, "ERROR: didn't read 8192 bytes from %s\n", argv[1]);
        exit (-3);
    }
    fclose (_f);

    _b = _b_raw;
    for (int c = 0; c < 1024; c ++) {
        int c2 = c * 8;
        *_b++ = _b_in[c2] + 2 * _b_in[c2 + 1] + 4 * _b_in[c2 + 2] +\
            8 * _b_in[c2 + 3] + 16 * _b_in[c2 + 4] + 32 * _b_in[c2 + 5] +\
            64 * _b_in[c2 + 6] + 128 * _b_in[c2 + 7];
    }

    printf ("// this file was automatically generated by raw2oled\n");
    printf ("#include <Arduino.h>\n");
    printf ("const uint8_t bmp [] PROGMEM = {\n");

    // for each of 128 8x8 blocks
    for (int c = 0; c < 128; c ++) {
        printf ("    ");
        _b = _b_raw + ((c / 16) * 128) + (c % 16);
        // horizontal scan
        for (int h = 0; h < 8; h ++) {
            uint8_t b_out = 0;
            // vertical scan
            for (int v = 0; v < 8; v ++)
                if (*(_b + (v * 16)) & (1 << h))
                    b_out |= (1 << v);
            
            printf ("0x%.2x, ", b_out);
        }
        printf ("\n");
    }
    printf ("};\n\n");
}

