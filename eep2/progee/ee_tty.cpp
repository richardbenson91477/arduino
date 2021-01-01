// termios flags c/o https://chrisheydrick.com/2012/06/17/how-to-read-serial-data-from-an-arduino-in-linux-with-c-part-3/
#include <string.h>

#include "ee_tty.h"

int ee_open () {
    struct termios ti;

    int ee = open (EE_TTY, O_RDWR | O_NOCTTY);
    if (ee < 0) {
        perror (EE_TTY);
        return -1;
    }

    tcgetattr (ee, &ti);
    // 9600
    cfsetospeed (&ti, B9600);     
    cfsetispeed (&ti, B9600);
    // 8N1
    ti.c_cflag &= ~PARENB;
    ti.c_cflag &= ~CSTOPB;
    ti.c_cflag &= ~CSIZE;
    ti.c_cflag |= CS8;
    // no hardware flow control
    ti.c_cflag &= ~CRTSCTS;
    // enable receiver, ignore status lines
    ti.c_cflag |= CREAD | CLOCAL;

    // disable input/output flow control, disable restart chars
    ti.c_iflag &= ~(IXON | IXOFF | IXANY);

    // disable canonical input, disable echo,
    // disable visually erase chars,
    // disable terminal-generated signals
    ti.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // disable output processing
    ti.c_oflag &= ~OPOST;
    
    // one character
    ti.c_cc[VMIN] = 1;
    ti.c_cc[VTIME] = 0;

    tcsetattr (ee, TCSANOW, &ti);

    // wait for board to reset
    usleep (1000000);

    tcflush (ee, TCIFLUSH);
    return ee;
}
 
int ee_await (int ee) {
    uint8_t ch;

    while (read (ee, &ch, 1) > 0) 
        if (ch == '?')
            return 1;

    return 0;
}

int ee_address (int ee, uint32_t a) {
    if (write (ee, "a", 1) <= 0)
        return 0;

    // assuming little-endian
    if (write (ee, &a, 2) <= 0)
        return 0;

    if (! ee_await (ee))
        return 0;

    fprintf (stderr, "address set to 0x%x\n", a);

    return 1;
}

