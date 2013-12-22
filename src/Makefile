.PHONY: clean, mrproper

.SUFFIXES:

CC = gcc
CFLAGS = -std=c99 -g

all: main.o file_io.o nonograms.o view.o
	$(CC) $^ -o picture $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf *.bak rm -rf *.o

mrproper: clean
	rm -rf picture
