#!/usr/bin/env python3
import sys, struct, serial

seq = ""
seqs = []

def main ():
    global seq, seqs

    if len(sys.argv) != 2:
        sys.stderr.write (
            "Usage: " +sys.argv[0] + " [[-s (serial)] | [outfile]]" + "\r\n")
        return

    if sys.argv[1] == "-s":
        sys.stdout.write ("Connecting.")
        sys.stdout.flush ()
        ser = serial.Serial ("/dev/ttyACM0", 9600, timeout=2)
        sys.stdout.write (".")
        sys.stdout.flush ()
        q = ser.read (1)
        while (q != '?'):
            sys.stdout.write (".")
            sys.stdout.flush ()
            q = ser.read (1)
        sys.stdout.write (".done.\r\n")
        sys.stdout.flush ()
    else:
        ser = False
        fn = sys.argv[1]
        try:
            f_out = file (fn, "wb")
        except (IOError, OSError) as e:
            sys.stderr.write (sys.argv[0] +": error: " +str (e) +"\r\n")
            return

    while True:
        si = sys.stdin.readline ()
        if not len(si):
            break

        s = si.split()
        if not s:
            s = ["?"]

        if s[0] == "q":
            break

        elif s[0] == "n":
            seqs += [seq]
            seq = ""
            seq2 = False

        elif s[0] == "f":
            if len(s) != 6:
                sys.stdout.write ("f freq1 freq2 step delay nrep\r\n")
                continue
            seq2 = "f" + struct.pack ("HHHHH", int (s[1]), int (s[2]),
                int (s[3]), int (s[4]), int (s[5]))
            seq += seq2

        elif s[0] == "p":
            if len(s) != 2:
                sys.stdout.write ("p freq")
                continue
            seq2 = "p" + struct.pack ("H", int (s[1]))
            seq += seq2

        elif s[0] == "P":
            if len(s) != 1:
                sys.stdout.write ("P")
                continue
            seq2 = "P"
            seq += seq2

        elif s[0] == "s":
            if len(s) != 2:
                sys.stdout.write ("s angle\r\n")
                continue
            seq2 = "s" + struct.pack ("B", int (s[1]))
            seq += seq2

        elif s[0] == "l":
            if len(s) != 3:
                sys.stdout.write ("l trig1 trig2\r\n")
                continue
            seq2 = "l" + struct.pack ("BB", int (s[1]), int (s[2]))
            seq += seq2

        elif s[0] == "m":
            if len(s) != 3:
                sys.stdout.write ("m trig1 trig2\r\n")
                continue
            seq2 = "m" + struct.pack ("BB", int (s[1]), int (s[2]))
            seq += seq2

        elif s[0] == "d":
            if len(s) != 2:
                sys.stdout.write ("d delay\r\n")
                continue
            seq2 = "d" + struct.pack ("H", int (s[1]))
            seq += seq2

        else:
            sys.stdout.write ("p/P/f/s/l/m/d/n/q\r\n")
            continue

        if ser and seq2:
            sys.stdout.write ("Sending.")
            sys.stdout.flush ()
            ser.write (seq2)
            sys.stdout.write (".")
            sys.stdout.flush ()
            q = ser.read (1)
            while (q != '*'):
                sys.stdout.write (".")
                sys.stdout.flush ()
                q = ser.read (1)
            sys.stdout.write (".done.\r\n")
            sys.stdout.flush ()

    if ser:
        ser.close ()

    else:
        if len(seq):
            seqs += [seq]

        f_out.write (struct.pack ("H", len(seqs)))

        for _seq in seqs:
            f_out.write (struct.pack ("H", len(_seq)))
        for _seq in seqs:
            f_out.write (_seq)

        f_out.close ()

if __name__ == '__main__':
    main()

