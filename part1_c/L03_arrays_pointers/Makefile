# Bài 03 - build: make | chạy: make run | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
PROGS  = arrays_pointers

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

run: all
	./arrays_pointers

clean:
	rm -f $(PROGS)

.PHONY: all run clean
