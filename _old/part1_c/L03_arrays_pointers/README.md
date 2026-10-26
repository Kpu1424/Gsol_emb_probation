# Bài 03 — Mảng, chuỗi, con trỏ, truyền dữ liệu vào hàm

## Mục tiêu
- Dùng mảng, tính số phần tử bằng `sizeof`, và biết **khi nào cách đó sai**.
- Hiểu con trỏ: `&` lấy địa chỉ, `*` truy cập giá trị, phép cộng con trỏ.
- Truyền mảng vào hàm (kèm độ dài), dùng con trỏ để hàm trả về nhiều kết quả, hiểu ý nghĩa `const`.
- Hiểu chuỗi C = mảng `char` kết thúc bằng `'\0'`; `strlen` khác `sizeof`; dùng `snprintf` an toàn.

## Cần biết trước
Bài 01, 02.

## Build và chạy
```bash
cd part1_c/L03_arrays_pointers
make && ./arrays_pointers
```

## Output dự kiến
- `sizeof(samples)` = 6 × `sizeof(int)`; số phần tử = 6.
- Trong `average`, `sizeof(data)` = kích thước **con trỏ** (thường 8 trên 64-bit, 4 trên 32-bit).
- Trung bình 255.5; min 248, max 270; sau offset −5: `samples[0] = 246`.
- Hai địa chỉ liên tiếp cách nhau `sizeof(int)`. Giá trị địa chỉ khác nhau mỗi lần chạy.
- `strlen("TEMP_01") = 7`, `sizeof(name) = 16`; sau sửa: `TEMP_91`; `label = TEMP_91=24.6C`.

## Kết quả thực tế đã chạy (2026-10-23, x86_64)
```
sizeof(samples) = 24 byte, so phan tu = 6
  [average] sizeof(data) = 8 (kich thuoc CON TRO, khong phai mang)
Trung binh = 255.5 (x0.1 C)
p tro toi samples[0]: *p = 251
*(p + 2) = 260 (giong samples[2])
Dia chi samples[0] = 0x7ffc23991290, samples[1] = 0x7ffc23991294 (cach nhau sizeof(int) = 4)
min = 248, max = 270
Sau apply_offset(-5): samples[0] = 246
name = "TEMP_01", strlen = 7, sizeof = 16
Sau khi sua name[5]: TEMP_91
label = TEMP_91=24.6C
```

## Giải thích dòng quan trọng
- `double average(const int *data, size_t n)`: mảng truyền vào hàm **suy biến** thành con trỏ. Hàm không biết độ dài, nên phải truyền `n`. `const` giúp compiler báo lỗi nếu hàm lỡ sửa dữ liệu.
- `find_min_max(samples, count, &min_v, &max_v)`: truyền **địa chỉ**. Trong hàm, `*out_min = ...` ghi thẳng vào `min_v` của `main`.
- `data[i]` ≡ `*(data + i)`. `p + 1` tiến **1 phần tử** (= `sizeof(int)` byte), không phải 1 byte.
- `char name[16] = "TEMP_01";` là mảng của bạn, sửa được. `const char *unit = "C";` trỏ tới chuỗi hằng, **sửa là hành vi không xác định** (thường crash).
- `snprintf(label, sizeof(label), ...)`: không bao giờ ghi quá `sizeof(label)` byte, và luôn có `'\0'`.

## Lỗi thường gặp
- `sizeof(arr)/sizeof(arr[0])` **bên trong hàm** nhận mảng: ra sai (kích thước con trỏ). gcc cảnh báo nếu tham số khai báo dạng `int data[]`.
- Truy cập `samples[6]` với mảng 6 phần tử (chỉ số 0..5): tràn mảng, hành vi không xác định.
- Con trỏ chưa khởi tạo: `int *p; *p = 1;` → crash hoặc hỏng bộ nhớ.
- `strcpy`/`sprintf` không giới hạn → tràn bộ đệm. Dùng `snprintf`.
- Quên chỗ cho `'\0'`: `char s[7] = "TEMP_01";` (7 ký tự + `'\0'` cần 8).

## Bài tập (chưa có đáp án)
1. Viết `size_t count_above(const int *data, size_t n, int threshold)`.
2. Viết `void reverse(int *data, size_t n)` đảo ngược mảng **tại chỗ**, chỉ dùng con trỏ (không dùng `[]`).
3. Viết `void swap_int(int *a, int *b)` rồi dùng nó trong `reverse`.
4. Thử bỏ `const` của `average` rồi thêm dòng `data[0] = 0;` vào trong hàm. Sau đó thêm lại `const`. Compiler nói gì?
5. Viết hàm tách chuỗi `"TEMP_01=25.3"` thành tên `"TEMP_01"` và giá trị `253` (gợi ý: `strchr`, `strtol`).

## Tự kiểm tra
- [ ] Giải thích vì sao `sizeof(data)` trong `average` là 8.
- [ ] Vẽ được ô nhớ của `min_v` và con trỏ `out_min` trỏ vào nó.
- [ ] Phân biệt `char s[] = "abc"` và `const char *s = "abc"`.
