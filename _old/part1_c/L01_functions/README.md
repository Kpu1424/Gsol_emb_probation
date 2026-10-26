# Bài 01 — Cấu trúc chương trình, hàm, tham số, giá trị trả về

## Mục tiêu
- Nhận ra các phần của một file C: `#include`, `#define`, khai báo hàm, `main`, định nghĩa hàm.
- Viết hàm có tham số và giá trị trả về; hiểu **truyền tham trị** (hàm nhận bản sao).
- Biết `main` trả về gì và cách xem mã thoát (`echo $?`).

## Cần biết trước
Biến, kiểu `int`/`float`, `printf`, `if`, `for` (file `hello.c` ở thư mục gốc).

## Build và chạy
```bash
cd part1_c/L01_functions
make            # = gcc -std=c11 -Wall -Wextra -g -O0 functions.c -o functions
./functions
echo $?         # mã thoát của main
```

## Output dự kiến
5 mẫu 25.0 / 31.0 / 37.0 / 43.0 / 49.0 C (raw = 250 + i*60, chia 10); **2** lần quá nhiệt (>40);
`value = 0` bên trong `try_reset` nhưng `counter` bên ngoài **vẫn = 7**; `echo $?` in `0`.

## Kết quả thực tế đã chạy (2026-10-23, gcc 11.4, không cảnh báo)
```
Mau 0: 25.0 C
Mau 1: 31.0 C
Mau 2: 37.0 C
Mau 3: 43.0 C
Mau 4: 49.0 C
So lan qua nhiet (> 40.0 C): 2
Trong try_reset, value = 0
Sau try_reset, counter van = 7
```

## Giải thích dòng quan trọng
- `int read_fake_raw(int sample_index);` là **khai báo** (prototype). Thân hàm ở cuối file là **định nghĩa**. Thiếu khai báo mà gọi hàm trước khi định nghĩa thì C11 báo lỗi/cảnh báo *implicit declaration*.
- `return raw / 10.0f;`: `10.0f` là số thực nên phép chia là chia thực. Viết `raw / 10` sẽ là **chia nguyên** (253/10 = 25).
- `return celsius > limit;`: biểu thức so sánh trong C có giá trị `1` hoặc `0`.
- `try_reset(counter)`: hàm nhận **bản sao** giá trị 7. Muốn hàm sửa được biến gốc phải truyền địa chỉ (Bài 03).
- `return 0;` trong `main`: báo cho shell/hệ điều hành là thành công. Khác 0 thường là lỗi.

## Lỗi thường gặp
- Quên `return` trong hàm không phải `void`: giá trị rác (`-Wall` cảnh báo *control reaches end of non-void function*).
- Kiểu trong khai báo khác kiểu trong định nghĩa: lỗi *conflicting types*.
- Dùng `%d` để in `float`: in sai, có cảnh báo `-Wformat`.
- Chia nguyên ngoài ý muốn: `1/2 == 0`.

## Bài tập (chưa có đáp án)
1. Viết hàm `float celsius_to_fahrenheit(float c)` và in thêm cột °F.
2. Sửa `main` để trả về `1` nếu có ít nhất một lần quá nhiệt. Kiểm tra bằng `echo $?`.
3. Viết hàm `int clamp(int value, int min, int max)` và dùng nó giới hạn raw trong [0, 450].
4. Xóa dòng khai báo của `is_overheat` ở đầu file, build lại. Đọc kỹ thông báo của gcc rồi giải thích.

## Tự kiểm tra
- [ ] Phân biệt được khai báo và định nghĩa hàm.
- [ ] Giải thích vì sao `counter` vẫn là 7.
- [ ] Biết `echo $?` dùng để làm gì.
