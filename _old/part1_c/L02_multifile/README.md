# Bài 02 — Khai báo / định nghĩa, tách file `.c` và `.h`

## Mục tiêu
- Tách chương trình thành module: `sensor.h` (giao diện) và `sensor.c` (cài đặt), dùng trong `main.c`.
- Hiểu `extern`, `static` ở mức file, include guard.
- Hiểu 2 bước **compile** (`.c` → `.o`) và **link** (`.o` → chương trình).

## Cần biết trước
Bài 01.

## File
- [sensor.h](sensor.h): `#define`, `extern int g_sensor_read_count;`, các prototype
- [sensor.c](sensor.c): định nghĩa biến toàn cục, mảng `static`, hàm `static` nội bộ
- [main.c](main.c): chỉ `#include "sensor.h"` và gọi hàm
- [Makefile](Makefile): compile từng file rồi link

## Build và chạy
```bash
cd part1_c/L02_multifile
make            # gcc -c main.c ; gcc -c sensor.c ; gcc main.o sensor.o -o sensor_app
./sensor_app
# Làm thủ công để thấy từng bước:
gcc -std=c11 -Wall -c sensor.c      # chỉ ra sensor.o
gcc -std=c11 -Wall -c main.c        # chỉ ra main.o
gcc main.o sensor.o -o sensor_app   # link
nm sensor.o                         # xem ký hiệu: T = hàm, D = biến có giá trị, t/b nhỏ = static
```

## Output dự kiến
Dòng `[sensor] init 4 kenh`. Kênh 0..3 có raw = (200 + ch*25) + số lần đọc. Kênh 4 báo lỗi. Tổng số lần đọc thành công = 4.

## Kết quả thực tế đã chạy (2026-10-23)
```
[sensor] init 4 kenh
Kenh 0: raw=201 -> 20.1 C
Kenh 1: raw=227 -> 22.7 C
Kenh 2: raw=253 -> 25.3 C
Kenh 3: raw=279 -> 27.9 C
Kenh 4: loi (kenh khong ton tai)
Tong so lan doc thanh cong: 4
```

## Giải thích dòng quan trọng
- `#ifndef SENSOR_H / #define SENSOR_H / #endif`: include guard. Nếu một file `.c` include header 2 lần (trực tiếp hoặc gián tiếp), nội dung chỉ được dùng 1 lần.
- `extern int g_sensor_read_count;` (trong `.h`) là **khai báo**: "biến này tồn tại ở đâu đó". `int g_sensor_read_count = 0;` (trong `.c`) là **định nghĩa**: cấp bộ nhớ. Toàn chương trình chỉ được có **một** định nghĩa.
- `static int s_base_raw[...]` và `static int is_valid_channel(...)` trong `sensor.c`: chỉ thấy được trong file đó (*internal linkage*). Đây là cách "đóng gói" trong C.
- `#include "sensor.h"` trong chính `sensor.c`: để compiler kiểm tra prototype khớp với định nghĩa.

## Lỗi thường gặp
| Thông báo | Nguyên nhân |
|---|---|
| `undefined reference to 'sensor_init'` (lỗi **link**) | Quên link `sensor.o`/`sensor.c` |
| `multiple definition of 'g_sensor_read_count'` | Đặt **định nghĩa** biến trong `.h` rồi include vào nhiều `.c` |
| `implicit declaration of function` | Quên `#include "sensor.h"` |
| `sensor.h: No such file or directory` | Sai thư mục; dùng `-I<thu_muc>` |
| Sửa `.h` nhưng chương trình không đổi | Makefile thiếu phụ thuộc vào `.h` (ở đây đã có `%.o: %.c sensor.h`) |

## Bài tập (chưa có đáp án)
1. Chuyển dòng `int g_sensor_read_count = 0;` từ `sensor.c` sang `sensor.h` (bỏ `extern`). Build, đọc lỗi, giải thích, rồi sửa lại.
2. Trong `main.c`, thử gọi `is_valid_channel(1)`. Lỗi xảy ra ở bước compile hay link? Vì sao?
3. Thêm module mới `filter.h`/`filter.c` có hàm `int filter_average(const int *data, int n)`. Cập nhật Makefile.
4. Xóa include guard trong `sensor.h`, rồi `#include "sensor.h"` 2 lần trong `main.c`. Có lỗi không? Thêm một `typedef` vào header rồi thử lại.

## Tự kiểm tra
- [ ] Phân biệt lỗi compile và lỗi link khi đọc thông báo.
- [ ] Giải thích `extern` và `static` (mức file) khác nhau thế nào.
- [ ] Biết cái gì nên đặt trong `.h` và cái gì trong `.c`.
