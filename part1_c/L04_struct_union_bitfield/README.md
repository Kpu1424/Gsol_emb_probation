# Bài 04 — struct, union, typedef, enum, bit field

## Mục tiêu
Sau bài này bạn:
1. Khai báo và dùng được `struct`, `typedef`, `enum` để mô tả một bản ghi cảm biến.
2. Chứng minh được bằng chương trình: thành viên struct **lưu riêng**, thành viên union **dùng chung** bộ nhớ.
3. Dùng được bit field, biết giá trị bị **cắt** khi vượt số bit, và biết vì sao với thanh ghi/giao thức người ta hay dùng **mask/shift**.
4. Đọc được `sizeof`/`offsetof` và giải thích **padding**, mà không coi các con số đó là cố định trên mọi máy.

## Cần biết trước
- Bài 01 (hàm), Bài 03 (con trỏ, mảng, `&`, `*`).
- Hệ hex: `0x99` = `1001 1001` (nhị phân); toán tử bit `&`, `|`, `~`, `<<`, `>>`.

## File

| File | Nội dung | Ý chính |
|---|---|---|
| [a_struct_enum_typedef.c](a_struct_enum_typedef.c) | struct + enum + typedef, `offsetof`, copy struct, con trỏ `->` | Lưu riêng, có padding |
| [b_union.c](b_union.c) | union 3 cách nhìn 4 byte; so với struct cùng thành viên; tagged union | Dùng chung, endianness |
| [c_bitfield.c](c_bitfield.c) | bit field thanh ghi trạng thái; giá trị bị cắt; mask/shift | Đóng gói bit |
| [d_compare_sizeof.c](d_compare_sizeof.c) | bảng `sizeof`; thứ tự thành viên ảnh hưởng padding; union bitfield + raw | So sánh |

## Build và chạy

```bash
cd part1_c/L04_struct_union_bitfield
make              # build cả 4 chương trình
make run          # chạy lần lượt cả 4
./b_union         # hoặc chạy từng cái
make clean
```

## Output dự kiến (điều PHẢI đúng trên mọi máy chuẩn C)

- **04a:** offset các thành viên **tăng dần và không chồng lên nhau**. `sizeof(struct)` **≥** tổng `sizeof` các thành viên. Sửa `value_x10` thì `id`, `type`, `timestamp_ms` giữ nguyên. `copy` sửa không ảnh hưởng `r`.
- **04b:** cả 3 thành viên union có **cùng địa chỉ, offset 0**. `sizeof(union)` **≥** thành viên lớn nhất. Ghi `u32` làm `bytes[]` thay đổi, và ngược lại. Ghi `f32` làm `u32` cũ không còn.
- **04c:** đọc lại đúng giá trị đã gán (khi giá trị vừa số bit). Gán `mode = 5` (2 bit) đọc lại được `1`; `channel = 20` (4 bit) đọc lại được `4`. Đây là phép lấy theo modulo 2^n với kiểu **unsigned**. Phần mask/shift cho cùng kết quả `enabled=1 error=0 mode=2 channel=9`.

**Không cố định** (phụ thuộc compiler/kiến trúc/ABI): giá trị cụ thể của `sizeof`/padding, `sizeof(enum)`, thứ tự byte (endianness), vị trí bit trong bit field, các "raw bytes", địa chỉ `%p` (thay đổi mỗi lần chạy).

## Kết quả thực tế đã chạy
Máy: Ubuntu 22.04 x86_64, gcc 11.4.0, `-std=c11 -O0`, ngày 2026-10-23. Build **không có cảnh báo**.

```
########## ./a_struct_enum_typedef
== 2. Moi thanh vien co vi tri (offset) RIENG ==
  offsetof id           = 0, sizeof = 1
  offsetof type         = 4, sizeof = 4
  offsetof value_x10    = 8, sizeof = 2
  offsetof timestamp_ms = 12, sizeof = 4
  Tong sizeof cac thanh vien = 11, sizeof(struct) = 16
== 3. Sua 1 thanh vien, cac thanh vien khac giu nguyen ==
  id=1 type=TEMP value=30.0 ts=1000 ms
== 4. Gan struct = COPY toan bo, 2 ban doc lap ==
  r   :  id=1 type=TEMP value=30.0 ts=1000 ms
  copy:  id=2 type=HUMI value=30.0 ts=1000 ms
== 5. Truyen con tro vao ham de sua struct goc ==
  id=1 type=TEMP value=28.5 ts=1000 ms
== 6. enum la so nguyen ==
  SENSOR_TEMP=0 SENSOR_HUMI=1 SENSOR_PRESS=2, sizeof(sensor_type_t)=4

########## ./b_union
== 1. Tat ca thanh vien bat dau o CUNG dia chi ==
  &w.u32   = 0x7ffe56c032c8 (offset 0)
  &w.f32   = 0x7ffe56c032c8 (offset 0)
  &w.bytes = 0x7ffe56c032c8 (offset 0)
== 2. sizeof: union vs struct cung thanh vien ==
  sizeof(union)  = 4  (>= thanh vien lon nhat)
  sizeof(struct) = 12  (>= tong cac thanh vien)
== 3. Ghi u32 roi doc bytes ...
  w.u32 = 0x11223344
  bytes[0..3] = 44 33 22 11
  -> May NAY la little-endian (byte thap nam truoc)
== 4. Sua 1 byte -> u32 cung doi ==
  sau w.bytes[0] = 0xFF: w.u32 = 0x112233FF
== 5. Ghi f32 -> u32 cu bi GHI DE ==
  w.f32 = 25.50, doc w.u32 = 0x41CC0000 (bit bieu dien float tren may nay)
== 6. Struct: sua 1 thanh vien KHONG anh huong thanh vien khac ==
  s.u32 = 0x11223344 (van giu nguyen), s.f32 = 25.50
== 7. Tagged union ...
  INT   1013
  FLOAT 25.30
  sizeof(sensor_value_t) = 8

########## ./c_bitfield
  sizeof(status_bits_t)  = 4
  sizeof(status_plain_t) = 16
  sau khi gan:                 enabled=1 error=0 mode=2 channel=9
  raw bytes: 99 00 00 00
  sau s.error = 1:             enabled=1 error=1 mode=2 channel=9
  raw bytes: 9B 00 00 00
  yeu cau mode=5, doc lai mode=1
  yeu cau channel=20, doc lai channel=4
  reg = 0x99
  enabled=1 error=0 mode=2 channel=9

########## ./d_compare_sizeof
uint8_t + uint32_t + uint8_t   6
struct {u8 a; u32 b; u8 c;}    12
struct {u32 b; u8 a; u8 c;}    8
union  {u8 a; u32 b; u8 c;}    4
bit field (1+4+3 bit)          4
bits a=1 b=0xA c=5 -> raw = 0x000000B5
```
(Rút gọn; output đầy đủ có trong `logs/` sau khi chạy `make check` ở thư mục gốc.)

## Giải thích những dòng quan trọng

### struct: lưu riêng
```c
typedef struct sensor_reading { uint8_t id; sensor_type_t type; int16_t value_x10; uint32_t timestamp_ms; } sensor_reading_t;
```
- Mỗi thành viên có **offset riêng**. Trên máy này `id` ở 0, `type` ở **4** (không phải 1), vì `type` 4 byte cần đặt ở địa chỉ chia hết cho 4. Byte 1..3 là **padding**.
- Tổng thành viên = 11 byte nhưng `sizeof` = 16: thêm padding giữa các thành viên và ở cuối, để mảng struct vẫn căn lề đúng.
- `r->value_x10` là cách viết gọn của `(*r).value_x10` khi `r` là con trỏ.
- `sensor_reading_t copy = r;` **sao chép toàn bộ** byte. Hai biến độc lập nhau.

### union: dùng chung
```c
typedef union { uint32_t u32; float f32; uint8_t bytes[4]; } raw_word_t;
```
- Mọi thành viên bắt đầu ở **offset 0**. `sizeof` = 4 = kích thước thành viên lớn nhất (có thể lớn hơn nếu cần căn lề).
- Ghi `u32 = 0x11223344` rồi đọc `bytes[0]` ra `0x44`: máy này **little-endian**. Chương trình **kiểm tra lúc chạy** chứ không giả định. Trên máy big-endian sẽ ra `0x11`.
- Đọc một thành viên khác thành viên vừa ghi ("type punning"): trong **C** được phép, nhưng giá trị phụ thuộc biểu diễn của máy (ví dụ `0x41CC0000` là dạng IEEE-754 của 25.5). Trong **C++** đây là **hành vi không xác định**; C++ dùng `memcpy` hoặc `std::bit_cast`.
- **Tagged union** (mục 7) là cách dùng an toàn: một trường `kind` cho biết thành viên nào đang hợp lệ.

### bit field: đóng gói bit
```c
typedef struct { unsigned int enabled:1; unsigned int error:1; unsigned int mode:2; unsigned int channel:4; } status_bits_t;
```
- 8 bit thông tin nhưng `sizeof` = 4 trên máy này: compiler cấp phát theo đơn vị `unsigned int`.
- Các trường **dùng chung byte** `0x99`, nhưng mỗi trường có **nhóm bit riêng**. Sửa `error` chỉ đổi 1 bit (`0x99` → `0x9B`).
- Với gcc trên x86_64, `enabled` nằm ở bit 0 (thấp nhất). Chuẩn C để **implementation-defined** thứ tự bit, cách gói, việc một trường có được vắt qua ranh giới đơn vị lưu trữ hay không. Compiler/kiến trúc khác có thể khác.
- Gán số lớn hơn số bit: **cắt** về n bit thấp, không có lỗi lúc chạy. Với hằng số, gcc cảnh báo `-Woverflow` (bạn đã thấy với `status.mode = 5` trong `hello.c` cũ). Với biến thì không cảnh báo.
- Vì không portable, với **thanh ghi phần cứng** và **gói tin giao thức** thường dùng `#define ..._MASK / _SHIFT` (mục 5 của `c_bitfield.c`): bạn tự quyết định vị trí bit.

### so sánh sizeof (04d)
- Cùng 3 thành viên `u8, u32, u8`: thứ tự `a,b,c` → 12 byte; thứ tự `b,a,c` → 8 byte trên máy này. **Sắp thành viên lớn trước** thường giảm padding.

## Lỗi thường gặp
| Lỗi | Hậu quả | Cách tránh |
|---|---|---|
| Giả định `sizeof(struct)` = tổng thành viên | Sai khi `memcpy`/gửi qua UART/ghi file | Luôn dùng `sizeof(kiểu)`, serialize từng trường |
| Gửi nguyên struct qua mạng/file giữa 2 máy khác nhau | Sai do padding + endianness | Định nghĩa định dạng byte rõ ràng |
| Đọc thành viên union không phải cái vừa ghi, và coi kết quả là portable | Khác nhau giữa các máy; UB trong C++ | Tagged union, hoặc `memcpy` |
| `&s.mode` với bit field | Lỗi biên dịch: *cannot take address of bit-field* | Copy ra biến thường trước |
| Bit field kiểu `int` (có dấu) 1 bit | Giá trị có thể là `-1` thay vì `1` | Dùng `unsigned int` |
| Dùng bit field để map thanh ghi phần cứng | Thứ tự bit đổi theo compiler | Mask/shift |
| Quên `typedef` rồi viết `sensor_reading_t` | Lỗi *unknown type name* | Hoặc dùng `struct sensor_reading` |
| `switch` trên enum thiếu `case` | Bỏ sót trạng thái | Bật `-Wall` (có `-Wswitch`), đọc cảnh báo |

## Bài tập (chưa có đáp án, tự sửa code rồi chạy lại)
1. **(04a)** Đổi thứ tự thành viên `sensor_reading_t` sao cho `sizeof` **nhỏ nhất có thể** trên máy này. Ghi lại `offsetof` từng thành viên trước và sau.
2. **(04a)** Thêm `SENSOR_LIGHT` vào enum nhưng **không** thêm `case` trong `type_name`. Build với `-Wall`: compiler nói gì? Sau đó sửa cho hết cảnh báo.
3. **(04b)** Viết hàm `uint32_t to_big_endian(uint32_t x)` dùng **dịch bit** (không dùng union). Kiểm tra `0x11223344` → `0x44332211` trên máy này.
4. **(04b)** Thêm kiểu `VALUE_TEXT` vào tagged union, chứa chuỗi tối đa 8 ký tự. `sizeof(sensor_value_t)` thay đổi thế nào? Vì sao?
5. **(04c)** Đổi `enabled` sang `int enabled : 1;` (có dấu), gán `1`, in ra. Giải thích kết quả.
6. **(04c)** Thêm trường `unsigned int reserved : 24;`. `sizeof` đổi không? Thêm tiếp `unsigned int extra : 4;` thì sao?
7. **(04c)** Viết hai hàm `status_pack(const status_bits_t*)` → `uint8_t` và `status_unpack(uint8_t, status_bits_t*)` dùng mask/shift. Kiểm tra pack rồi unpack ra đúng giá trị ban đầu.
8. **(04d)** Thử `__attribute__((packed))` (phần mở rộng của gcc, không phải chuẩn C) cho `order_bad_t`. `sizeof` là bao nhiêu? Tìm hiểu vì sao packed có thể làm truy cập chậm hoặc lỗi trên một số CPU ARM.

## Tự kiểm tra (tự trả lời, không nhìn code)
- [ ] Viết được một struct 3 trường và dự đoán `offsetof` từng trường **trên x86_64 gcc**. Chạy để kiểm.
- [ ] Giải thích bằng 1 câu: vì sao ghi `w.u32` làm thay đổi `w.bytes[0]`, nhưng ghi `s.u32` không làm thay đổi `s.bytes[0]`.
- [ ] Nói được 3 điều chuẩn C **không** đảm bảo về bit field.
- [ ] Biết khi nào chọn bit field, khi nào chọn mask/shift.
- [ ] Đã tự làm ít nhất bài tập 1, 3, 5, 7 và giải thích được kết quả.

> Code mẫu chạy được **không** có nghĩa là bạn đã đạt bài này. Bạn đạt khi tự làm được các bài tập và trả lời được phần tự kiểm tra.
