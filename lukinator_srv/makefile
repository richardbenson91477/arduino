CXXFLAGS=-std=c++14 -Ofast -march=native -fPIC -fomit-frame-pointer -funroll-loops -fno-rtti -fno-exceptions
OBJS=lukinator_srv.o nitrogen/net/net.a nitrogen/dlog/dlog.a
LDLIBS=-lm

all: lukinator_srv

lukinator_srv: $(OBJS)

test: lukinator_srv
	./lukinator_srv

clean:
	rm -f lukinator_srv *.o

