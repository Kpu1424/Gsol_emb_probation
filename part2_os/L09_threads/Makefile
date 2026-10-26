# Bài 09 - build: make | chạy: make run | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0 -pthread
PROGS  = threads

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

run: all
	./threads

clean:
	rm -f $(PROGS)

.PHONY: all run clean
