ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(ROOT_DIR)/builder-head.mak

ARD_FQBN=-fqbn arduino:avr:uno
ARD_HW+=
ARD_PREFS+=
ARD_UPLOAD=avrdude -C /etc/avrdude.conf -P /dev/ttyACM0 -c arduino -p m328p -D -U flash:w:$(TARGET_OUT):i
ARD_PROGRAM=avrdude -C /etc/avrdude.conf -b 19200 -c usbtiny -p m328p -U flash:w:$(TARGET_OUT)

TARGET_SRC=$(TARGET).cpp
TARGET_OUT=build/$(TARGET_SRC).with_bootloader.hex

include $(ROOT_DIR)/builder-common.mak

