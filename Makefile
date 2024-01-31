cc := gcc
CFLAGS := -Wall -Wextra -Werror -O3
LIBS := -lz

all: main

main:main.o seqio.o
	$(cc) $(CFLAGS) -o main $^ $(LIBS)

main.o:main.c
	$(cc) $(CFLAGS) $(LIBS) -c $<

seqio.o:seqio.c
	$(cc) $(CFLAGS) $(LIBS) -c $<

clean:
	rm -f main.o seqio.o

.PHONY:clean
