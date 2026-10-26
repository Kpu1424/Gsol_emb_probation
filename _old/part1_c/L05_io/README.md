# Bài 05 — Nhập/xuất bàn phím, đọc/ghi file

## Mục tiêu
- Đọc input an toàn: `fgets` + `strtol` (kiểm tra lỗi), xử lý EOF (Ctrl+D).
- Phân biệt `stdout` và `stderr`.
- Ghi/đọc file text (CSV) bằng `fopen/fprintf/fgets/sscanf/fclose`. Ghi/đọc nhị phân bằng `fwrite/fread`.
- Kiểm tra lỗi mở file bằng `perror`.

## Cần biết trước
Bài 03 (chuỗi, con trỏ), Bài 04 (struct, padding).

## Build và chạy
```bash
cd part1_c/L05_io
make
./a_keyboard_io                                  # gõ tay; Ctrl+D để kết thúc input
printf '40\n25\nabc\n45\nq\n' | ./a_keyboard_io  # dữ liệu có sẵn
./b_file_io && cat sensor_log.csv
xxd sensor_log.bin | head                        # xem file nhị phân
make run
```

## Output dự kiến
- `a_keyboard_io` với input mẫu: ngưỡng 40; `abc` bị bỏ qua; `45` sinh CẢNH BÁO; tổng kết "2 gia tri hop le, 1 canh bao, trung binh 35.00". Input rỗng (EOF ngay) → in "Khong co du lieu nhap", mã thoát 1.
- `b_file_io`: tạo `sensor_log.csv` có header + 5 dòng; đọc lại thấy 5 bản ghi, trung bình 25.60 C. File nhị phân: 5 bản ghi × `sizeof(record_t)` byte. Mở file không tồn tại → `perror` in `No such file or directory`.

## Kết quả thực tế đã chạy (2026-10-23)
```
########## printf '40\n25\nabc\n45\nq\n' | ./a_keyboard_io
Nhap nguong canh bao (do C): Gia tri cam bien (q de dung): Gia tri cam bien (q de dung):   Bo qua 'abc' (khong hop le)
Gia tri cam bien (q de dung):   CANH BAO: 45 > 40
Gia tri cam bien (q de dung):
Da nhap 2 gia tri hop le, 1 canh bao, trung binh 35.00

########## ./b_file_io
fopen doc CSV: No such file or directory          <- xem "Lỗi thường gặp" về thứ tự dòng này
== Ghi CSV: sensor_log.csv ==
== Doc lai CSV ==
  dong 1 bo qua: timestamp_ms,temp_x10
  t=    0 ms  temp=25.0 C
  ...
  t= 4000 ms  temp=26.2 C
  -> 5 ban ghi, trung binh 25.60 C
== Binary: sensor_log.bin ==
  fwrite: 5 ban ghi x 8 byte = 40 byte
  fread : 5 ban ghi, in[4].temp_x10 = 262
== Thu mo file khong ton tai ==
```
EOF test: `printf '' | ./a_keyboard_io` → `Khong co du lieu nhap. Thoat.`, `exit=1`.

## Giải thích dòng quan trọng
- `fgets(buf, size, stdin)`: đọc **cả dòng**, không bao giờ vượt `size`. Trả về `NULL` khi EOF/lỗi.
- `strtol(text, &end, 10)` + kiểm tra `end == text` (không có số), `*end != '\0'` (có ký tự thừa), `errno == ERANGE` (tràn). `scanf("%d")` không làm được các kiểm tra này và để rác trong bộ đệm khi gặp `abc`.
- `fflush(stdout)` sau lời nhắc: `stdout` có bộ đệm. Nếu không xả, lời nhắc có thể chưa hiện khi chương trình chờ nhập.
- `fprintf(stderr, ...)` / `perror(...)`: thông báo lỗi đi kênh riêng, nên `./prog > out.txt` vẫn thấy lỗi trên màn hình.
- `sscanf(line, "%lu,%d", ...) != 2`: luôn kiểm tra **số trường đọc được**. Header CSV bị bỏ qua nhờ điều này.
- `fwrite(out, sizeof(record_t), N, f)`: ghi nguyên **ảnh bộ nhớ**, gồm cả padding và thứ tự byte của máy này. Trên máy này `sizeof(record_t)` = 8 (4 + 2 + 2 byte padding).

## Lỗi thường gặp
- **Thứ tự output lẫn lộn**: ở kết quả trên, dòng `perror` (stderr) xuất hiện **đầu tiên**, dù được gọi cuối cùng. Khi output đi qua pipe/file, `stdout` được đệm theo khối và chỉ xả lúc thoát, còn `stderr` thì không đệm. Chạy thẳng trên terminal sẽ thấy đúng thứ tự.
- Không kiểm tra `fopen` trả `NULL` → crash khi `fprintf(NULL, ...)`.
- Quên `fclose` → dữ liệu chưa xuống đĩa, rò rỉ file descriptor.
- `while (!feof(f))` để đọc file → lặp thừa một lần. Hãy kiểm tra giá trị trả về của `fgets`/`fread`.
- Mở `"w"` nhầm file cần giữ → **mất nội dung cũ**. Muốn ghi thêm thì dùng `"a"`.
- `scanf("%s")` không giới hạn độ dài → tràn bộ đệm.

## Bài tập (chưa có đáp án)
1. Sửa `a_keyboard_io` để từ chối ngưỡng ngoài khoảng [-40, 125] và hỏi lại.
2. Sửa `b_file_io` để **ghi thêm** (append) 5 bản ghi mỗi lần chạy thay vì ghi đè. Header chỉ ghi khi file mới.
3. Viết hàm ghi file nhị phân **portable**: mỗi bản ghi ghi đúng 6 byte, little-endian, tự tách byte bằng dịch bit. Viết hàm đọc tương ứng.
4. Chạy `./b_file_io > out.txt`. Dòng lỗi nằm ở đâu? Thử `./b_file_io > out.txt 2> err.txt`.
5. Thêm cột `humidity` vào CSV, và xử lý dòng hỏng (thiếu cột) mà không dừng chương trình.

## Tự kiểm tra
- [ ] Giải thích vì sao `fgets + strtol` an toàn hơn `scanf`.
- [ ] Biết khi nào dùng `stderr`.
- [ ] Giải thích vì sao file `.bin` có thể đọc sai trên máy khác.
