#!/usr/bin/env python
import sys, serial

com = serial.Serial(sys.argv[1], 1200)
com.dtr=False
com.close()

