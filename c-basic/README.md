# C cơ bản (L01 → L05)

Mỗi file `.c` minh họa đúng **một** ý, có `main` riêng và chạy độc lập. Mỗi bài có một `README.md` gồm: code đầy đủ, output thật và giải thích, chép thẳng vào report được.

Môi trường đã kiểm tra: Ubuntu 22.04, gcc 11.4.0, GNU Make 4.3. Cờ build: `-std=c11 -Wall -Wextra -g -O0`.

```bash
make L01        # build + chạy mọi file của L01, lưu output thật vào L01_functions/output/
make all        # cả 5 bài
make clean      # xóa build/ và output/ của mọi bài
```

## Bảng tiến độ

| Đầu ra | Bài | Cách chạy | Tiêu chí đạt | Code mẫu | Bạn |
|---|---|---|---|---|---|
| Hàm, tham số, giá trị trả về, cấu trúc chương trình | [L01](L01_functions/README.md) | `make L01` | Tự viết lại `02_pass_by_value.c` và `04_main_return_code.c` không nhìn code; giải thích tham trị và `echo $?` | 4 file, build + chạy, 0 warning | Chưa đạt |
| Khai báo/định nghĩa, tách `.c`/`.h`, `extern`, `static` | [L02](L02_multifile/README.md) | `make L02` | Nhìn thông báo lỗi biết là lỗi compile hay link; giải thích `extern`/`static`; gõ tay 3 lệnh gcc build `01_basic` | 5 ví dụ, chạy được; 3 lỗi cố ý tái hiện đúng | Chưa đạt |
| Địa chỉ, con trỏ, mảng, chuỗi | [L03](L03_arrays_pointers/README.md) | `make L03` | Tự viết `swap` và `reverse` bằng con trỏ; giải thích `sizeof` mảng trong hàm | 8 file, build + chạy; 1 warning cố ý | Chưa đạt |
| struct, union, typedef, enum, bit field | [L04](L04_struct_union_bitfield/README.md) | `make L04` | Dự đoán đúng `sizeof` có padding trước khi chạy; giải thích struct khác union; giải thích vì sao `mode = 5` đọc ra 1 | 7 file, build + chạy; 1 warning cố ý | Chưa đạt |
| Nhập/xuất bàn phím, đọc/ghi file | [L05](L05_io/README.md) | `make L05` | `fgets` + `strtol` có kiểm lỗi; ghi/đọc CSV; giải thích vì sao file `.bin` không portable | 6 file, build + chạy, 0 warning | Chưa đạt |

"Code mẫu chạy được" **không** có nghĩa là bạn đã đạt bài. Chỉ đổi cột **Bạn** sau khi tự làm được tiêu chí đạt và trả lời được phần "Tự kiểm tra" của bài.

## Thứ tự học đề xuất

L01 → L03 → L04 → L02 → L05

Mỗi file chỉ dùng kiến thức của bài hiện tại và các bài học trước nó theo thứ tự này. Ví dụ: L04 dùng con trỏ của L03; L05 dùng chuỗi của L03 và struct của L04.

## Quy ước

- Đầu mỗi file `.c` có 3 dòng: `Ý chính`, `Build` (lệnh gcc gõ tay được, không cần make), `Quan sát`.
- `_wrong.c`: **file sai cố ý**, đi cặp với `_right.c` (hoặc bản đúng tương ứng).
- **Warning cố ý** (được phép, ghi rõ trong comment đầu file và trong README của bài): `L03/06_sizeof_in_function.c`, `L04/06_bitfield_register.c`. Warning thật được lưu ở đầu `output/<tên>.txt`.
- Output có địa chỉ bộ nhớ hoặc đường dẫn tuyệt đối sẽ khác trên máy bạn.
- Code cũ (trước khi tái cấu trúc) nằm ở `../_old/part1_c/`.
