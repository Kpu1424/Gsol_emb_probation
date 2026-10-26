# Phần 1 — C nền tảng (L01 → L05)

Ví dụ xuyên suốt: dữ liệu cảm biến giả lập. Mỗi bài có `README.md` (mục tiêu, build/run, output dự kiến, kết quả thực tế, giải thích, lỗi thường gặp, bài tập, tự kiểm tra) và `Makefile` (`make`, `make run`, `make clean`).

Môi trường đã kiểm tra: Ubuntu 22.04.5 x86_64, gcc 11.4.0, GNU Make 4.3. Cờ build: `-std=c11 -Wall -Wextra -g -O0`.

| Đầu ra | Bài | File | Cách chạy | Tiêu chí đạt | Code mẫu | Bạn |
|---|---|---|---|---|---|---|
| Cấu trúc chương trình, hàm, tham số, giá trị trả về | [L01](L01_functions/README.md) | `functions.c` | `cd L01_functions && make run` | Làm bài tập 1–4, giải thích tham trị và `echo $?` | Đã build + chạy, không cảnh báo | Chưa đạt |
| Khai báo/định nghĩa, tách `.c`/`.h` | [L02](L02_multifile/README.md) | `sensor.h`, `sensor.c`, `main.c` | `cd L02_multifile && make run` | Phân biệt lỗi compile/link, `extern`/`static`; bài tập 1–4 | Đã build + chạy | Chưa đạt |
| Mảng, chuỗi, con trỏ, truyền dữ liệu vào hàm | [L03](L03_arrays_pointers/README.md) | `arrays_pointers.c` | `cd L03_arrays_pointers && make run` | Viết `reverse`/`swap` bằng con trỏ; giải thích `sizeof` trong hàm | Đã build + chạy | Chưa đạt |
| struct, union, typedef, enum, bit field | [L04](L04_struct_union_bitfield/README.md) | `a_…`, `b_union.c`, `c_bitfield.c`, `d_compare_sizeof.c` | `cd L04_struct_union_bitfield && make run` | Bài tập 1, 3, 5, 7; trả lời phần tự kiểm tra | Đã build + chạy cả 4 | Chưa đạt |
| Nhập/xuất bàn phím, đọc/ghi file | [L05](L05_io/README.md) | `a_keyboard_io.c`, `b_file_io.c` | `cd L05_io && make run` | `fgets`+`strtol` có kiểm lỗi; ghi/đọc CSV; giải thích vì sao file `.bin` không portable | Đã build + chạy (kể cả trường hợp EOF) | Chưa đạt |

"Code mẫu chạy được" **không** có nghĩa là bạn đã đạt bài. Chỉ đổi cột **Bạn** sau khi bạn tự làm bài tập và trả lời được phần tự kiểm tra.

C++: các bài minh họa bằng C. Điểm khác quan trọng đã được ghi chú trong L04: đọc thành viên union không phải thành viên vừa ghi là hành vi không xác định trong C++.

## Thứ tự học đề xuất
L04 (theo yêu cầu, học trước) → L01 → L02 → L03 → L05. Nếu thấy L04 khó ở phần con trỏ `->`/`&`, hãy học L03 trước.
