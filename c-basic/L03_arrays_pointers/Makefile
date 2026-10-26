# L03 - make: build + chạy mọi file .c, lưu output thật vào output/ | make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0

SRCS = $(wildcard *.c)
OUTS = $(SRCS:%.c=output/%.txt)

all: $(OUTS)

# Bước 1: biên dịch. Warning của gcc vừa in ra màn hình vừa lưu vào build/<tên>.warn
build/%: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) $< -o $@ 2> build/$*.warn; status=$$?; cat build/$*.warn; exit $$status

# Bước 2: chạy, lưu output thật. File có warning cố ý: chép warning lên đầu file output
output/%.txt: build/%
	@mkdir -p output
	@if [ -s build/$*.warn ]; then echo "=== Warning khi build (co y) ==="; cat build/$*.warn; echo "=== Output khi chay ==="; fi > $@
	./$< >> $@
	@cat $@

clean:
	rm -rf build output

.PRECIOUS: build/%
.PHONY: all clean
