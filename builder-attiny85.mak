ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(ROOT_DIR)/builder-head.mak

ARD_FQBN=-fqbn attiny:avr:attinyx5:chip=85,clock=16pll,LTO=disable,millis=disabled,TimerClockSource=default
ARD_HW+=
ARD_PREFS=
ARD_UPLOAD=
ARD_PROGRAM=avrdude -C /etc/avrdude.conf -b 19200 -c usbtiny -p t85 -U flash:w:$(TARGET_OUT)

TARGET_SRC=$(TARGET).cpp
TARGET_OUT=build/$(TARGET_SRC).hex

# 16mhz internal oscillator: -U lfuse:w:0xf1:m 

include $(ROOT_DIR)/builder-common.mak

