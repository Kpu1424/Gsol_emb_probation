# Bài 08 - build: make | chạy: make run | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
PROGS  = fork_wait fork_exec sensor_child

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

run: all
	@echo "########## ./fork_wait";  ./fork_wait
	@echo
	@echo "########## ./fork_exec";  ./fork_exec

clean:
	rm -f $(PROGS)

.PHONY: all run clean
