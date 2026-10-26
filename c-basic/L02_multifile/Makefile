# L02 - make: build + chạy + lưu output thật vào output/, kể cả lỗi compile/link cố ý | make clean
#   make compile_error : chỉ chạy ví dụ lỗi compile (02)
#   make link_error    : chỉ chạy ví dụ lỗi link (03)
CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0

all: output/01_basic.txt output/02_compile_error.txt output/03_link_error.txt \
     output/04_extern.txt output/05_static_right.txt output/05_static_wrong.txt

compile_error: output/02_compile_error.txt
link_error: output/03_link_error.txt

# Bước COMPILE: mỗi file .c thành một file .o (chưa phải chương trình chạy được)
build/obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Sửa header thì phải compile lại các file .c include nó
build/obj/01_basic/main.o build/obj/01_basic/sensor.o: 01_basic/sensor.h
build/obj/04_extern/main.o build/obj/04_extern/config.o: 04_extern/config.h
build/obj/05_static/main_right.o build/obj/05_static/counter.o: 05_static/counter.h

# Bước LINK: nối các file .o thành một chương trình
build/01_basic: build/obj/01_basic/main.o build/obj/01_basic/sensor.o
	$(CC) $^ -o $@
build/04_extern: build/obj/04_extern/main.o build/obj/04_extern/config.o
	$(CC) $^ -o $@
build/05_static_right: build/obj/05_static/main_right.o build/obj/05_static/counter.o
	$(CC) $^ -o $@

# Chạy chương trình, lưu output thật
output/%.txt: build/%
	@mkdir -p output
	./$< > $@
	@cat $@

# ===== Các ví dụ LỖI CỐ Ý =====
# Dấu "!" đảo kết quả: target chỉ ĐẠT khi gcc THẤT BẠI đúng như mong đợi.
# File output chứa lệnh đã gõ (dòng "$ ...") và thông báo lỗi thật của gcc.

# 02: lỗi COMPILE (dùng biến chưa khai báo)
CMD_02 = $(CC) $(CFLAGS) -c 02_compile_error/main_wrong.c -o build/main_wrong.o
output/02_compile_error.txt: 02_compile_error/main_wrong.c 01_basic/sensor.h
	@mkdir -p build output
	@echo '$$ $(CMD_02)' > $@
	! $(CMD_02) 2>> $@
	@cat $@

# 03: lỗi LINK (link main.o nhưng quên sensor.o)
CMD_03 = $(CC) build/obj/01_basic/main.o -o build/03_link_error
output/03_link_error.txt: build/obj/01_basic/main.o
	@mkdir -p output
	@echo '$$ $(CMD_03)' > $@
	! $(CMD_03) 2>> $@
	@cat $@

# 05 (sai): lỗi LINK vì count là static trong counter.c
CMD_05 = $(CC) build/obj/05_static/main_wrong.o build/obj/05_static/counter.o -o build/05_static_wrong
output/05_static_wrong.txt: build/obj/05_static/main_wrong.o build/obj/05_static/counter.o
	@mkdir -p output
	@echo '$$ $(CMD_05)' > $@
	! $(CMD_05) 2>> $@
	@cat $@

clean:
	rm -rf build output

.PHONY: all clean compile_error link_error
