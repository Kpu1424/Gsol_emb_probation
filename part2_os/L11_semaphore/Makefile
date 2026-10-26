# Bài 11 - build: make | chạy: make run | kiểm tra race bằng TSan: make tsan | dọn: make clean
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0 -pthread
PROGS  = producer_consumer producer_consumer_tsan

all: $(PROGS)

producer_consumer: producer_consumer.c
	$(CC) $(CFLAGS) $< -o $@
producer_consumer_tsan: producer_consumer.c
	$(CC) $(CFLAGS) -fsanitize=thread $< -o $@

run: producer_consumer
	./producer_consumer

tsan: producer_consumer_tsan
	@setarch $$(uname -m) -R ./producer_consumer_tsan 2>&1 | grep -E 'WARNING|SUMMARY|trung binh' || true

clean:
	rm -f $(PROGS)

.PHONY: all run tsan clean
