#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

extern int ee_open ();
extern int ee_await (int ee);
extern int ee_address (int ee, uint32_t a);

