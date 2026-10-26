# Bài 06 - build: make | chạy: make run | xem kích thước section: make sections | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
PROGS  = memory_layout

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

run: all
	./memory_layout

# size: tổng kích thước text/data/bss trong FILE thực thi (chưa chạy)
# nm  : ký hiệu nằm ở section nào (T=text, D=data, B=bss, R=rodata)
sections: all
	size memory_layout
	@echo
	nm memory_layout | grep -E ' (g_|read_sensor|s_threshold|s_call_count)'

clean:
	rm -f $(PROGS)

.PHONY: all run sections clean
