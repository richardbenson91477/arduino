ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(ROOT_DIR)/builder-head.mak

ARD_FQBN=-fqbn arduino:avr:micro
ARD_HW+=
ARD_PREFS+=
ARD_UPLOAD=../reset-leo.py /dev/ttyACM? && sleep 1 && avrdude -C /etc/avrdude.conf -P /dev/ttyACM? -c avr109 -p m32u4 -b 57600 -D -U flash:w:$(TARGET_OUT):i
ARD_PROGRAM=avrdude -C /etc/avrdude.conf -b 19200 -c usbtiny -p m32u4 -V -U flash:w:$(TARGET_OUT)

TARGET_SRC=$(TARGET).cpp
TARGET_OUT=build/$(TARGET_SRC).with_bootloader.hex

include $(ROOT_DIR)/builder-common.mak

