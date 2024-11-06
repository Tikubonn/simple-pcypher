
export GCC=gcc
export CFLAGS+=-I$(SIMPLE_PCYPHER_INCLUDE) -L$(SIMPLE_PCYPHER_LIB) -g

.PHONY: all
all: bin/test
	gdb bin/test

.PHONY: test-bin
test-bin: scripts/test-simple-pcypher.bash scripts/test.bash
	env PATH="$(PATH):$(SIMPLE_PCYPHER_BIN)" ./scripts/test-simple-pcypher.bash

.PHONY: clean
clean: 
	rm -f src/test-simple-pcypher-calc-encrypted-data-size.o
	rm -f src/test-simple-pcypher-calc-key.o
	rm -f src/test-simple-pcypher-encrypt-decrypt.o
	rm -f src/main.o

src/test-simple-pcypher-calc-encrypted-data-size.o: src/test-simple-pcypher-calc-encrypted-data-size.c src/test-simple-pcypher-calc-encrypted-data-size.h $(SIMPLE_PCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-pcypher-calc-encrypted-data-size.o src/test-simple-pcypher-calc-encrypted-data-size.c 

src/test-simple-pcypher-calc-key.o: src/test-simple-pcypher-calc-key.c src/test-simple-pcypher-calc-key.h $(SIMPLE_PCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-pcypher-calc-key.o src/test-simple-pcypher-calc-key.c 

src/test-simple-pcypher-encrypt-decrypt.o: src/test-simple-pcypher-encrypt-decrypt.c src/test-simple-pcypher-encrypt-decrypt.h src/test.h $(SIMPLE_PCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-pcypher-encrypt-decrypt.o src/test-simple-pcypher-encrypt-decrypt.c

src/main.o: src/main.c src/test-simple-pcypher-calc-encrypted-data-size.h src/test-simple-pcypher-calc-key.h src/test-simple-pcypher-encrypt-decrypt.h $(SIMPLE_PCYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/main.o src/main.c

bin: 
	mkdir -p bin

bin/test: src/test-simple-pcypher-calc-encrypted-data-size.o src/test-simple-pcypher-calc-key.o src/test-simple-pcypher-encrypt-decrypt.o src/main.o $(SIMPLE_PCYPHER_INCLUDE) $(SIMPLE_PCYPHER_LIB) | bin
	$(GCC) $(CFLAGS) -o bin/test src/test-simple-pcypher-calc-encrypted-data-size.o src/test-simple-pcypher-calc-key.o src/test-simple-pcypher-encrypt-decrypt.o src/main.o -lsimplepcypher
