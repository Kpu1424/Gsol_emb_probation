# Bài 01 - build: make | chạy: make run | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
PROGS  = functions

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

run: all
	./functions

clean:
	rm -f $(PROGS)

.PHONY: all run clean
