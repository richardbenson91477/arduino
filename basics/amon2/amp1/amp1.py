#!/usr/bin/env python3

import serial
import struct

ser = None
buf = []

def await ():
    global ser

    x = ser.read()
    while x is not '?':
        x = ser.read()
    if x is not '?':
        print("invalid serial input")

def amp1 ():
    global ser, buf

    ser = serial.Serial ("/dev/ttyACM0", 9600, timeout=2)
    if not ser: 
        print ("ser err")
        return

    await ()

    getn (1024)
    await ()

    ser.close ()
    plot (buf)

def get256 ():
    global ser, buf

    buf = []
    ser.write ("r")
    for cl in range (256):
        buf += struct.unpack ("H", ser.read() + ser.read())

def getn (n):
    global ser, buf

    ns = "n" + struct.pack ("H", n)

    buf = []
    ser.write (ns)
    for cl in range (n):
        buf += struct.unpack ("H", ser.read () + ser.read())

