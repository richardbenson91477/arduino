#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/io.h>

static uint16_t PP = 0x378;
#ifndef ODATA
	#define ODATA 1
	#define OCLK 128
	#define IDATA 64
#endif

#define CLKDP 1
#define CLKDH 1

inline uint8_t i_read (void) {
	return (! (inb (PP +1) & IDATA));
}

inline void o_write (uint8_t w) {
	if (w) {
		outb (ODATA, PP);
		usleep (CLKDP);

		outb (OCLK | ODATA, PP);
		usleep (CLKDH);

		outb (ODATA, PP);
	} 
	else {
		outb (0, PP);
		usleep (CLKDP);

		outb (OCLK, PP);
		usleep (CLKDH);

		outb (0, PP);
	}
}

void o_write_uint8 (uint8_t d) {
	o_write (d & 1);
	o_write (d & 2);
	o_write (d & 4);
	o_write (d & 8);
	o_write (d & 16);
	o_write (d & 32);
	o_write (d & 64);
	o_write (d & 128);
}

int init (void) {
	int res;

	res = ioperm (PP, 3, 1);
	if (res) return res;

	outb (0, PP);
}

int main (void) {
	int c;

	if (init ()) {
		fprintf (stderr, "init failed\n");
		return -1;
	}
/*
	printf ("waiting for request."); fflush (stdout);
	while (! i_read ()) {
		usleep (20000); 
		printf ("."); fflush (stdout);
	}
*/
	printf ("\nrelaying.\n"); fflush (stdout);

	while ((c = fgetc (stdin)) >= 0) {
		o_write_uint8 (c);
	}

	printf ("\ndone.\n");
	return 0;
}

