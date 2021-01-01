#ifndef EE_TTY
  EE_TTY="/dev/ttyACM1"
#endif

CXXFLAGS+="-DEE_TTY=\"$(EE_TTY)\""

all: progee diffee dumpee

progee: ee_tty.o
diffee: ee_tty.o
dumpee: ee_tty.o

clean:
	rm -f progee diffee dumpee ee_tty.o

