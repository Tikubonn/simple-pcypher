
export GCC=gcc
export CFLAGS=-Wall
# export CFLAGS=-Wall -g -Og #for debug build.
# export CFLAGS=-Wall -O3 -finline-functions #for release build.

all: dist/include/simple-pcypher/simple-pcypher.h dist/lib/libsimplepcypher.a dist/lib/libsimplepcypher.so bin/simple-pcypher

.PHONY: clean
clean:
	rm -f src/simple-pcypher.o
	rm -f src/main.o
	make -C test clean

.PHONY: test
test: all 
	make -C test SIMPLE_PCYPHER_INCLUDE=$(CURDIR)/dist/include SIMPLE_PCYPHER_LIB=$(CURDIR)/dist/lib

src/simple-pcypher.o: src/simple-pcypher.c src/simple-pcypher.h
	$(GCC) $(CFLAGS) -c -o src/simple-pcypher.o src/simple-pcypher.c

dist/include/simple-pcypher:
	mkdir -p dist/include/simple-pcypher

dist/include/simple-pcypher/simple-pcypher.h: src/simple-pcypher.h | dist/include/simple-pcypher
	cp src/simple-pcypher.h dist/include/simple-pcypher/simple-pcypher.h

dist/lib:
	mkdir -p dist/lib

dist/lib/libsimplepcypher.a: src/simple-pcypher.o | dist/lib
	ar r dist/lib/libsimplepcypher.a src/simple-pcypher.o

dist/lib/libsimplepcypher.so: src/simple-pcypher.o | dist/lib
	$(GCC) $(CFLAGS) -shared -o dist/lib/libsimplepcypher.so src/simple-pcypher.o

bin:
	mkdir -p bin

src/main.o: src/main.c src/buffer.h src/simple-pcypher.h
	$(GCC) $(CFLAGS) -Idist/include -c -o src/main.o src/main.c

bin/simple-pcypher: src/main.o dist/include/simple-pcypher/simple-pcypher.h dist/lib/libsimplepcypher.a | bin
	$(GCC) $(CFLAGS) -Ldist/lib -o bin/simple-pcypher src/main.o -lsimplepcypher
