# Bài 07 - build: make | chạy: make run | xem lỗi bằng ASan: make bugs | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
PROGS  = lifetime_heap heap_bugs

all: $(PROGS)

lifetime_heap: lifetime_heap.c
	$(CC) $(CFLAGS) $< -o $@

# AddressSanitizer: compiler chèn mã kiểm tra truy cập bộ nhớ lúc chạy
heap_bugs: heap_bugs.c
	$(CC) $(CFLAGS) -fsanitize=address -fno-omit-frame-pointer $< -o $@

run: lifetime_heap
	./lifetime_heap 4

# Mỗi lỗi chạy riêng; "|| true" để make không dừng khi ASan báo lỗi (exit code != 0)
bugs: heap_bugs
	@for m in leak uaf double overflow; do \
	  echo "########## ./heap_bugs $$m"; ./heap_bugs $$m 2>&1 | grep -E 'ERROR|SUMMARY|Doc sau|Da lam' || true; echo; \
	done

clean:
	rm -f $(PROGS)

.PHONY: all run bugs clean
