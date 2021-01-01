
all: $(TARGET_OUT)

upload: $(TARGET_OUT)
	$(ARD_UPLOAD)

program: $(TARGET_OUT)
	$(ARD_PROGRAM)

build:
	mkdir -p build

$(TARGET_OUT): $(TARGET_SRC) build $(TARGET_DEPS)
	$(ARD_HOME)/arduino-builder -verbose -compile $(ARD_HW) $(ARD_TOOLS) $(ARD_LIBS) $(ARD_PREFS) -prefs runtime.tools.avr-gcc.path=$(ARD_HOME)/hardware/tools/avr/ $(ARD_FQBN) -build-path $(shell pwd)/build $(TARGET_SRC)

clean:
	rm -rf build/

