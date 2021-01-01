#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "ee_tty.h"

int main (int argc, char *argv[]) {
    if (argc < 3) {
        fprintf (stderr, "usage: %s address size\n", argv[0]);
        return -1;
    }
    uint32_t address = atoi (argv[1]);
    uint32_t dump_n = atoi (argv[2]);

    int ee = ee_open ();
    if (ee < 0) {
        fprintf (stderr, "serial open failed\n");
        return -2;
    }

    if (! ee_address (ee, address))
        return -3;

    fprintf (stderr, "beginning dump of 0x%x bytes\n", dump_n);

    uint32_t c;
    uint8_t ch;
    for (c = 0; c < dump_n; c ++) {
        if (write (ee, "b", 1) <= 0)
            break;

        if (read (ee, &ch, 1) <= 0)
            break;

        if (! ee_await (ee))
            break;

        write (1, &ch, 1);

        if (write (ee, "n", 1) <= 0)
            break;

        if (! ee_await (ee))
            break;
    }

    fprintf (stderr, "0x%x (%d) bytes read\n", c, c);
    close (ee);
    return 0;
}

