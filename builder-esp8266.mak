ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
include $(ROOT_DIR)/builder-head.mak

ARD_FQBN=-fqbn "esp:esp8266:generic:xtal=160,vt=flash,stacksmash=disabled,ssl=all,ResetMethod=nodemcu,CrystalFreq=26,FlashFreq=40,FlashMode=dout,eesz=1M64,led=2,sdk=nonosdk_190703,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=115200"

ARD_HW+=-hardware $(ARD_HOME)/hardware/esp/esp8266
ARD_PREFS+=

TARGET_SRC=$(TARGET).cpp
TARGET_OUT=build/$(TARGET_SRC).bin

include $(ROOT_DIR)/builder-common.mak

