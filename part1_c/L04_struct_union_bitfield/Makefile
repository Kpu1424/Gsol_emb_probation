# Bài 04 - build: make | chạy tất cả: make run | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
PROGS  = a_struct_enum_typedef b_union c_bitfield d_compare_sizeof

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

run: all
	@for p in $(PROGS); do echo "########## ./$$p"; ./$$p; echo; done

clean:
	rm -f $(PROGS)

.PHONY: all run clean
