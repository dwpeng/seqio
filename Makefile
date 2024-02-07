export cc := gcc
export LIBS := -lz -lm
export ROOT_DIR := $(shell pwd)
export INCLUDE := $(ROOT_DIR)
export CFLAGS := -Wall -Wextra -Werror -O3 -I$(INCLUDE)
export seqioSource := $(ROOT_DIR)/seqio.c 
export seqioLIB := $(seqioSource:.c=.o)

all: main build-test

main:main.o seqio.o
	$(cc) $(CFLAGS) -o main $^ $(LIBS)

main.o:main.c
	$(cc) $(CFLAGS) $(LIBS) -c $<

seqio.o: $(seqioSource)
	$(cc) $(CFLAGS) $(LIBS) -c $<

build-test:
	$(MAKE) -C ./test

clean:
	rm -f main.o seqio.o main test-seqio test-kseq

.PHONY:clean
