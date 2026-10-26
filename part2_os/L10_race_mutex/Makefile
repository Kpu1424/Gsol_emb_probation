# Bài 10
#   make          : build race, mutex (+ bản TSan)
#   make run      : chạy race và mutex 1 lần
#   make race5    : chạy race 5 lần để thấy kết quả thay đổi
#   make tsan     : ThreadSanitizer phát hiện data race (kể cả khi kết quả "đúng")
#   make clean
CC     = gcc
# -O0: giữ nguyên thao tác đọc/ghi bộ nhớ để dễ quan sát. Race vẫn là LỖI ở mọi mức tối ưu.
CFLAGS = -std=c11 -Wall -Wextra -g -O0 -pthread
PROGS  = race mutex race_tsan mutex_tsan

all: $(PROGS)

race: race.c
	$(CC) $(CFLAGS) $< -o $@
mutex: mutex.c
	$(CC) $(CFLAGS) $< -o $@
race_tsan: race.c
	$(CC) $(CFLAGS) -fsanitize=thread $< -o $@
mutex_tsan: mutex.c
	$(CC) $(CFLAGS) -fsanitize=thread $< -o $@

run: race mutex
	@echo "########## ./race";  ./race
	@echo "########## ./mutex"; ./mutex

race5: race
	@for i in 1 2 3 4 5; do ./race; done

# Trên máy này (kernel 6.8 + gcc 11), TSan cần tắt ASLR cho process bằng "setarch -R",
# nếu không sẽ báo "FATAL: ThreadSanitizer: unexpected memory mapping".
TSAN_RUN = setarch $$(uname -m) -R
tsan: race_tsan mutex_tsan
	@echo "########## race_tsan (mong doi: WARNING data race)"
	@$(TSAN_RUN) ./race_tsan 2>&1 | grep -E 'WARNING|SUMMARY|Mong doi' || true
	@echo "########## mutex_tsan (mong doi: khong co canh bao)"
	@$(TSAN_RUN) ./mutex_tsan 2>&1 | grep -E 'WARNING|SUMMARY|Mong doi' || true

clean:
	rm -f $(PROGS)

.PHONY: all run race5 tsan clean
