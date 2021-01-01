ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(ROOT_DIR)/builder-head.mak

ARD_FQBN=-fqbn arduino:avr:mega:cpu=atmega2560
ARD_HW+=
ARD_PREFS+=
ARD_UPLOAD=avrdude -C /etc/avrdude.conf -P /dev/ttyACM1 -c wiring -p m2560 -D -U flash:w:$(TARGET_OUT):i
ARD_PROGRAM=avrdude -C /etc/avrdude.conf -b 19200 -c usbtiny -p m2560 -U flash:w:$(TARGET_OUT)

TARGET_SRC=$(TARGET).cpp
TARGET_OUT=build/$(TARGET_SRC).with_bootloader.hex

include $(ROOT_DIR)/builder-common.mak

