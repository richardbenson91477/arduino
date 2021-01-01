TARGET=raw2oled

$(TARGET): $(TARGET).o
	$(CXX) -o $@ $^

test: $(TARGET)
	./$(TARGET) osh.data

clean:
	rm -f $(TARGET) $(TARGET).o

