# Bài 02 - Biên dịch TỪNG file .c thành .o, rồi LIÊN KẾT (link) thành 1 chương trình
#   make        : build
#   make run    : chạy
#   make clean  : dọn
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0
OBJS   = main.o sensor.o

sensor_app: $(OBJS)
	$(CC) $(OBJS) -o $@          # bước LINK

# Bước COMPILE: mỗi .c -> .o; nếu sensor.h đổi thì build lại
%.o: %.c sensor.h
	$(CC) $(CFLAGS) -c $< -o $@

run: sensor_app
	./sensor_app

clean:
	rm -f $(OBJS) sensor_app

.PHONY: run clean
