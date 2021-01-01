
ARD_HOME=/prebuilt/arduino
ARD_HW=-hardware $(ARD_HOME)/hardware
ARD_TOOLS=-tools $(ARD_HOME)/tools-builder
ARD_LIBS=-libraries $(ARD_HOME)/libraries

ifndef TARGET
  TARGET=$(notdir $(shell pwd))
endif

