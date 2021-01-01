ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(ROOT_DIR)/builder-head.mak

ARD_FQBN=-fqbn stm:stm32:GenF1:pnum=BLUEPILL_F103C8
ARD_HW+=-hardware $(ARD_HOME)/packages/hardware
ARD_TOOLS+=-tools $(ARD_HOME)/packages/tools
ARD_PREFS+=
ARD_UPLOAD=st-flash --format ihex write build/$(TARGET).cpp.hex

TARGET_SRC=$(TARGET).cpp
TARGET_OUT=build/$(TARGET_SRC).bin

include $(ROOT_DIR)/builder-common.mak

