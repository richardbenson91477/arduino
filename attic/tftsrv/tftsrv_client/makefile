all: to_565 vgrab

to_565: to_565.o
	$(CXX) -o $@ $^
	strip $@

vgrab: vgrab.o
	$(CXX) -o $@ $^ -lv4l2
	strip $@

clean:
	rm -f to_565 to_565.o vgrab vgrab.o
