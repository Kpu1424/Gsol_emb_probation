# Bài 05 - build: make | chạy: make run | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
PROGS  = a_keyboard_io b_file_io

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

# a_keyboard_io chạy với dữ liệu mẫu qua pipe; muốn gõ tay thì chạy ./a_keyboard_io
run: all
	@echo "########## printf '40\\n25\\nabc\\n45\\nq\\n' | ./a_keyboard_io"
	@printf '40\n25\nabc\n45\nq\n' | ./a_keyboard_io
	@echo
	@echo "########## ./b_file_io"
	@./b_file_io

clean:
	rm -f $(PROGS) sensor_log.csv sensor_log.bin

.PHONY: all run clean
