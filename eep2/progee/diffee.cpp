#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "ee_tty.h"

int main (int argc, char *argv[]) {
    int ee = ee_open ();
    if (ee < 0) {
        fprintf (stderr, "serial open failed\n");
        return -1;
    }

    uint32_t address = 0;
    if (argc > 1)
        address = atoi (argv[1]);

    if (! ee_address (ee, address))
        return -2;

    uint32_t n = 0;
    uint8_t ch, ch2;
    while (read (0, &ch, 1) > 0) {
        write (1, ".", 1);

        if (write (ee, "b", 1) <= 0)
            break;

        if (read (ee, &ch2, 1) <= 0)
            break;

        if (! ee_await (ee))
            break;

        if (ch != ch2) {
            fprintf (stderr, "compare failed at 0x%x+0x%x: 0x%x != 0x%x\n", address, n, ch, ch2);
            close (ee);
            return -4;
        }

        if (write (ee, "n", 1) <= 0)
            break;
        
        if (! ee_await (ee))
            break;

        n ++;
    }

    fprintf (stderr, "\n0x%x (%d) bytes compared\n", n, n);
    close (ee);
    return 0;
}

