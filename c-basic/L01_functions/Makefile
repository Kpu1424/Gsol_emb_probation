# L01 - make: build + chạy mọi file .c, lưu output thật vào output/ | make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0

SRCS = $(wildcard *.c)
OUTS = $(SRCS:%.c=output/%.txt)

all: $(OUTS)

# Bước 1: biên dịch mỗi file .c thành một chương trình trong build/
build/%: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) $< -o $@

# Bước 2: chạy chương trình, lưu output thật vào output/<tên>.txt
output/%.txt: build/%
	@mkdir -p output
	./$< > $@
	@cat $@

# Riêng 04: main cố ý trả về 1, nên ghi thêm mã thoát giống như gõ "echo $?"
output/04_main_return_code.txt: build/04_main_return_code
	@mkdir -p output
	./$< > $@; code=$$?; echo '$$ echo $$?' >> $@; echo $$code >> $@
	@cat $@

clean:
	rm -rf build output

.PRECIOUS: build/%
.PHONY: all clean
