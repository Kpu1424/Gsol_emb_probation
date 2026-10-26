# L05 - make: build + chạy mọi file .c, lưu output thật vào output/ | make clean
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

# File đọc bàn phím (01, 02, 03): lấy dữ liệu nhập từ file .in thay cho gõ tay
output/01_printf_scanf.txt output/02_scanf_wrong.txt output/03_fgets_strtol_right.txt: output/%.txt: build/% %.in
	@mkdir -p output
	./$< < $*.in > $@
	@cat $@

# 04: ghi thêm nội dung file CSV vừa tạo, giống như gõ "cat sensor.csv"
output/04_write_csv.txt: build/04_write_csv
	@mkdir -p output
	./$< > $@
	echo '$$ cat sensor.csv' >> $@; cat sensor.csv >> $@
	@cat $@

# 05 đọc file do 04 tạo ra, nên phải chạy sau 04
output/05_read_csv.txt: output/04_write_csv.txt

clean:
	rm -rf build output sensor.csv record.bin

.PRECIOUS: build/%
.PHONY: all clean
