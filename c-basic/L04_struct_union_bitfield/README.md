# L04 - struct, union, typedef, enum, bit field

Ý chính: `struct` gom nhiều thành viên, mỗi thành viên có vùng nhớ riêng (có thể có thêm padding). `union` cho mọi thành viên dùng chung một vùng nhớ. Bit field cho mỗi thành viên chỉ chiếm vài bit, rất hợp để mô phỏng thanh ghi. `typedef` đặt tên mới cho kiểu, còn `enum` đặt tên cho các hằng số nguyên.

| File | Ý minh họa | Lệnh chạy |
|------|------------|-----------|
| [01_struct_basic.c](01_struct_basic.c) | struct: thành viên lưu riêng | `make && ./build/01_struct_basic` |
| [02_typedef.c](02_typedef.c) | typedef đặt tên mới cho kiểu | `make && ./build/02_typedef` |
| [03_enum.c](03_enum.c) | enum: hằng số có tên | `make && ./build/03_enum` |
| [04_padding_sizeof.c](04_padding_sizeof.c) | Padding: thứ tự thành viên đổi thì sizeof đổi | `make && ./build/04_padding_sizeof` |
| [05_union.c](05_union.c) | union dùng chung bộ nhớ | `make && ./build/05_union` |
| [06_bitfield_register.c](06_bitfield_register.c) | Bit field mô phỏng thanh ghi (**warning cố ý**) | `make && ./build/06_bitfield_register` |
| [07_struct_pointer_arrow.c](07_struct_pointer_arrow.c) | `->` với con trỏ struct | `make && ./build/07_struct_pointer_arrow` |

## 01_struct_basic.c
```c
/*
 * Ý chính: struct gom nhiều biến liên quan thành một bản ghi; mỗi thành viên có vùng nhớ riêng.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 01_struct_basic.c -o 01_struct_basic
 * Quan sát: sửa s.temp thành 30 thì s.id và s.voltage vẫn giữ nguyên.
 */
#include <stdio.h>

struct sensor {
  int id;
  int temp;      /* độ C */
  int voltage;   /* mV */
};

int main(void) {
  struct sensor s = {1, 25, 3300};   /* khởi tạo theo đúng thứ tự khai báo: id, temp, voltage */

  printf("Ban dau:          id = %d, temp = %d, voltage = %d\n", s.id, s.temp, s.voltage);
  s.temp = 30;                       /* dấu chấm: truy cập một thành viên */
  printf("Sau khi sua temp: id = %d, temp = %d, voltage = %d\n", s.id, s.temp, s.voltage);
  printf("sizeof(struct sensor) = %zu byte (3 thanh vien int x 4 byte)\n", sizeof(struct sensor));
  return 0;
}
```
Output:
```text
Ban dau:          id = 1, temp = 25, voltage = 3300
Sau khi sua temp: id = 1, temp = 30, voltage = 3300
sizeof(struct sensor) = 12 byte (3 thanh vien int x 4 byte)
```
Giải thích: Mỗi thành viên `id`, `temp`, `voltage` có một chỗ riêng trong struct, nên gán `s.temp = 30` chỉ đổi đúng 4 byte của `temp`. 3 thành viên `int` × 4 byte = 12 byte, khớp với `sizeof`. Struct này không có padding vì mọi thành viên cùng kích thước (xem `04`).

## 02_typedef.c
```c
/*
 * Ý chính: typedef đặt TÊN MỚI cho một kiểu đã có, ví dụ "sensor_t" thay cho "struct sensor".
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 02_typedef.c -o 02_typedef
 * Quan sát: a (struct sensor) và b (sensor_t) là cùng một kiểu nên "b = a" chép được; millivolt_t có sizeof bằng int.
 */
#include <stdio.h>

struct sensor {
  int id;
  int temp;
};

typedef struct sensor sensor_t;   /* từ đây viết sensor_t cũng chính là struct sensor */
typedef int millivolt_t;          /* typedef cho kiểu cơ bản: tên nói rõ đơn vị, kiểu vẫn là int */

int main(void) {
  struct sensor a = {1, 25};
  sensor_t b;
  millivolt_t v = 3300;

  b = a;   /* cùng kiểu nên gán trực tiếp được: chép toàn bộ thành viên */
  printf("a: id = %d, temp = %d\n", a.id, a.temp);
  printf("b: id = %d, temp = %d (chep tu a)\n", b.id, b.temp);
  printf("v = %d mV, sizeof(millivolt_t) = %zu, sizeof(int) = %zu\n", v, sizeof(millivolt_t), sizeof(int));
  return 0;
}
```
Output:
```text
a: id = 1, temp = 25
b: id = 1, temp = 25 (chep tu a)
v = 3300 mV, sizeof(millivolt_t) = 4, sizeof(int) = 4
```
Giải thích: `typedef` không tạo kiểu mới mà chỉ đặt thêm một tên. `sensor_t` và `struct sensor` là một kiểu, nên `b = a` hợp lệ và chép toàn bộ thành viên. `millivolt_t` vẫn là `int` (cùng `sizeof` = 4); lợi ích là tên kiểu cho người đọc biết đơn vị.

## 03_enum.c
```c
/*
 * Ý chính: enum đặt tên cho một nhóm hằng số nguyên; mặc định bắt đầu từ 0 và tăng dần 1.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 03_enum.c -o 03_enum
 * Quan sát: MODE_OFF = 0, MODE_NORMAL = 1, MODE_FAST = 2; switch dùng tên dễ đọc thay cho số 0, 1, 2.
 */
#include <stdio.h>

typedef enum {
  MODE_OFF,      /* 0 */
  MODE_NORMAL,   /* 1 */
  MODE_FAST      /* 2 */
} sensor_mode_t;

int main(void) {
  sensor_mode_t mode = MODE_FAST;

  printf("MODE_OFF = %d, MODE_NORMAL = %d, MODE_FAST = %d\n", MODE_OFF, MODE_NORMAL, MODE_FAST);
  printf("mode hien tai = %d\n", mode);

  switch (mode) {   /* -Wall cảnh báo nếu thiếu case cho một giá trị của enum */
    case MODE_OFF:
      printf("Cam bien dang tat\n");
      break;
    case MODE_NORMAL:
      printf("Doc 1 lan moi giay\n");
      break;
    case MODE_FAST:
      printf("Doc 10 lan moi giay\n");
      break;
  }
  return 0;
}
```
Output:
```text
MODE_OFF = 0, MODE_NORMAL = 1, MODE_FAST = 2
mode hien tai = 2
Doc 10 lan moi giay
```
Giải thích: Compiler tự gán 0, 1, 2 cho các tên theo thứ tự khai báo, nên `mode` lưu số 2. Viết `case MODE_FAST:` dễ đọc hơn `case 2:`. Nếu sau này thêm một mode mới mà quên thêm `case`, `-Wall` sẽ cảnh báo.

## 04_padding_sizeof.c
```c
/*
 * Ý chính: Compiler chèn byte trống (padding) để int nằm ở địa chỉ chia hết cho 4; đổi thứ tự thành viên thì sizeof có thể đổi.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 04_padding_sizeof.c -o 04_padding_sizeof
 * Quan sát: dữ liệu thật chỉ 6 byte, nhưng order_a = 12 byte, order_b = 8 byte (số liệu của gcc x86_64, máy khác có thể khác).
 */
#include <stdio.h>
#include <stddef.h>   /* offsetof: vị trí (byte thứ mấy) của thành viên tính từ đầu struct */

struct order_a { char a; int b; char c; };   /* nhỏ - lớn - nhỏ */
struct order_b { int b; char a; char c; };   /* lớn trước, nhỏ sau */

int main(void) {
  printf("Du lieu that: char + int + char = %zu byte\n", sizeof(char) + sizeof(int) + sizeof(char));

  printf("struct order_a {char a; int b; char c;}: sizeof = %zu\n", sizeof(struct order_a));
  printf("  offset a = %zu, b = %zu, c = %zu\n",
         offsetof(struct order_a, a), offsetof(struct order_a, b), offsetof(struct order_a, c));

  printf("struct order_b {int b; char a; char c;}: sizeof = %zu\n", sizeof(struct order_b));
  printf("  offset b = %zu, a = %zu, c = %zu\n",
         offsetof(struct order_b, b), offsetof(struct order_b, a), offsetof(struct order_b, c));
  return 0;
}
```
Output (số liệu của gcc trên x86_64; compiler/kiến trúc khác có thể khác):
```text
Du lieu that: char + int + char = 6 byte
struct order_a {char a; int b; char c;}: sizeof = 12
  offset a = 0, b = 4, c = 8
struct order_b {int b; char a; char c;}: sizeof = 8
  offset b = 0, a = 4, c = 5
```
Giải thích: CPU đọc `int` nhanh và an toàn nhất khi `int` nằm ở địa chỉ chia hết cho 4. Trong `order_a`, sau `a` (offset 0) compiler chèn 3 byte trống để `b` bắt đầu ở offset 4. Sau `c` (offset 8) lại thêm 3 byte để `sizeof` = 12, chia hết cho 4, nhờ vậy mảng các struct vẫn thẳng hàng. Trong `order_b`, `int` đứng đầu và hai `char` xếp liền nhau ở offset 4, 5, chỉ cần thêm 2 byte cuối, nên `sizeof` = 8.

## 05_union.c
```c
/*
 * Ý chính: Mọi thành viên của union dùng CHUNG một vùng nhớ: ghi vào thành viên này thì đọc thành viên kia thấy thay đổi.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 05_union.c -o 05_union
 * Quan sát: 2 địa chỉ giống nhau; sizeof = 4; ghi value = 0x11223344 thì bytes = 44 33 22 11 (máy little-endian). Địa chỉ sẽ khác trên máy bạn.
 */
#include <stdio.h>

union word {
  unsigned int value;       /* nhìn 4 byte như 1 số (unsigned int = 4 byte trên máy này) */
  unsigned char bytes[4];   /* nhìn chính 4 byte đó như 4 ô riêng */
};

int main(void) {
  union word w;

  printf("Dia chi w.value: %p\n", (void *)&w.value);
  printf("Dia chi w.bytes: %p\n", (void *)&w.bytes);
  printf("sizeof(union word) = %zu byte (bang thanh vien lon nhat, khong phai tong)\n", sizeof(union word));

  w.value = 0x11223344;
  printf("Ghi w.value = 0x%08X\n", w.value);
  /* bytes[0] = 44: byte thấp nằm ở địa chỉ thấp -> máy little-endian */
  printf("Doc w.bytes = %02X %02X %02X %02X\n", w.bytes[0], w.bytes[1], w.bytes[2], w.bytes[3]);

  w.bytes[0] = 0xFF;   /* sửa 1 byte cũng là sửa một phần của value */
  printf("Sau khi ghi w.bytes[0] = 0xFF: w.value = 0x%08X\n", w.value);
  return 0;
}
```
Output (địa chỉ sẽ khác trên máy bạn):
```text
Dia chi w.value: 0x7ffcbc0fa784
Dia chi w.bytes: 0x7ffcbc0fa784
sizeof(union word) = 4 byte (bang thanh vien lon nhat, khong phai tong)
Ghi w.value = 0x11223344
Doc w.bytes = 44 33 22 11
Sau khi ghi w.bytes[0] = 0xFF: w.value = 0x112233FF
```
Giải thích: Hai thành viên có cùng địa chỉ: chúng là hai cách nhìn vào cùng 4 byte, nên `sizeof` = 4 chứ không phải 8. Ghi `0x11223344` rồi đọc từng byte ra `44 33 22 11`: byte thấp nhất (`44`) nằm ở địa chỉ thấp nhất, máy x86_64 là **little-endian**. Sửa `bytes[0]` thì byte thấp của `value` đổi theo, thành `0x112233FF`. Trên máy big-endian, thứ tự byte sẽ ngược lại.

## 06_bitfield_register.c
**Warning cố ý**: gcc cảnh báo `-Woverflow` ở dòng `reg.bits.mode = 5;` vì 5 không vừa 2 bit. Warning thật được lưu ở đầu file output.
```c
/*
 * Ý chính: Bit field cho mỗi thành viên chỉ chiếm vài bit, dùng để mô phỏng thanh ghi trạng thái; giá trị vượt số bit bị CẮT.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 06_bitfield_register.c -o 06_bitfield_register
 * Quan sát: sizeof = 4 dù chỉ dùng 8 bit; raw = 0x99; gán mode = 5 (nhị phân 101) đọc lại chỉ còn 1 (nhị phân 01), raw thành 0x95.
 * WARNING CỐ Ý: gcc báo -Woverflow ở dòng "reg.bits.mode = 5" vì 5 không vừa 2 bit.
 */
#include <stdio.h>

struct status_reg {
  unsigned int enable  : 1;   /* 1 bit: 0..1  */
  unsigned int error   : 1;   /* 1 bit: 0..1  */
  unsigned int mode    : 2;   /* 2 bit: 0..3  */
  unsigned int channel : 4;   /* 4 bit: 0..15 */
};

/* Xem cùng vùng nhớ như một số nguyên, giống đọc nguyên thanh ghi một lần */
union status_view {
  struct status_reg bits;
  unsigned int raw;
};

int main(void) {
  union status_view reg;
  reg.raw = 0;   /* xóa hết bit về 0 */

  printf("sizeof(struct status_reg) = %zu byte\n", sizeof(struct status_reg));

  reg.bits.enable = 1;
  reg.bits.mode = 2;
  reg.bits.channel = 9;
  printf("enable = %u, error = %u, mode = %u, channel = %u -> raw = 0x%02X\n",
         reg.bits.enable, reg.bits.error, reg.bits.mode, reg.bits.channel, reg.raw);

  reg.bits.mode = 5;   /* 5 cần 3 bit, trường chỉ có 2 bit -> chỉ giữ 2 bit thấp (01) */
  printf("Sau khi gan mode = 5: mode = %u, raw = 0x%02X\n", reg.bits.mode, reg.raw);
  return 0;
}
```
Output (warning khi build + output khi chạy):
```text
=== Warning khi build (co y) ===
06_bitfield_register.c: In function ‘main’:
06_bitfield_register.c:34:19: warning: unsigned conversion from ‘int’ to ‘unsigned char:2’ changes value from ‘5’ to ‘1’ [-Woverflow]
   34 |   reg.bits.mode = 5;   /* 5 cần 3 bit, trường chỉ có 2 bit -> chỉ giữ 2 bit thấp (01) */
      |                   ^
=== Output khi chay ===
sizeof(struct status_reg) = 4 byte
enable = 1, error = 0, mode = 2, channel = 9 -> raw = 0x99
Sau khi gan mode = 5: mode = 1, raw = 0x95
```
Giải thích: 4 trường chỉ dùng 1+1+2+4 = 8 bit, nhưng compiler cấp nguyên một `unsigned int` (4 byte) để chứa chúng. Với gcc trên x86_64, `enable` nằm ở bit 0, `mode` ở bit 2–3, `channel` ở bit 4–7, nên raw = `1001 1001` = 0x99. Gán 5 (nhị phân `101`) vào trường 2 bit thì chỉ giữ 2 bit thấp `01` = 1, raw thành 0x95. Lúc chạy không có lỗi nào, chỉ compiler cảnh báo. Trong warning, gcc gọi trường này là `unsigned char:2`: đó là cách gcc mô tả nội bộ một trường 2 bit, không phải lỗi trong code.

## 07_struct_pointer_arrow.c
```c
/*
 * Ý chính: Với con trỏ tới struct, p->temp là cách viết gọn của (*p).temp; truyền con trỏ để hàm sửa được struct gốc.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 07_struct_pointer_arrow.c -o 07_struct_pointer_arrow
 * Quan sát: 3 cách đọc đều ra 25; sau calibrate(&s, -2) thì s.temp trong main thành 23.
 */
#include <stdio.h>

typedef struct {
  int id;
  int temp;
} sensor_t;

void calibrate(sensor_t *p, int offset) {
  p->temp = p->temp + offset;   /* sửa struct GỐC qua địa chỉ, không phải bản sao */
}

int main(void) {
  sensor_t s = {1, 25};
  sensor_t *p = &s;

  printf("s.temp    = %d\n", s.temp);
  printf("(*p).temp = %d\n", (*p).temp);   /* cần ngoặc: *p.temp sẽ bị hiểu là *(p.temp) */
  printf("p->temp   = %d\n", p->temp);

  calibrate(&s, -2);
  printf("Sau calibrate(&s, -2): s.temp = %d\n", s.temp);
  return 0;
}
```
Output:
```text
s.temp    = 25
(*p).temp = 25
p->temp   = 25
Sau calibrate(&s, -2): s.temp = 23
```
Giải thích: `p` chứa địa chỉ của `s`, nên `*p` là chính `s`. Ba cách `s.temp`, `(*p).temp` và `p->temp` cùng đọc một ô nhớ; `->` chỉ là cách viết gọn của `(*p).`. `calibrate` nhận địa chỉ nên sửa thẳng `s` của `main` (giống `04_swap_right.c` ở L03), và không phải chép cả struct vào hàm.

## Lỗi hay gặp
- Nghĩ rằng `sizeof(struct)` bằng tổng `sizeof` các thành viên, rồi dùng con số đó khi ghi file hoặc gửi qua UART. Luôn dùng `sizeof(kiểu)` (xem `04`).
- Ghi vào một thành viên union rồi đọc thành viên khác, và tin rằng máy nào cũng cho cùng kết quả: kết quả phụ thuộc endianness (xem `05`).
- Dùng bit field để ánh xạ thanh ghi phần cứng thật: vị trí bit do compiler quyết định. Code driver thật thường dùng phép dịch bit và mask (`<<`, `&`, `|`) để tự kiểm soát vị trí bit.

## Tự kiểm tra
1. Vì sao sửa `s.temp` không làm đổi `s.id`, nhưng sửa `w.bytes[0]` lại làm đổi `w.value`?
2. Trước khi chạy, hãy dự đoán `sizeof` của `struct { char a; char b; int c; }` trên máy này, rồi giải thích.
3. `typedef` có tạo ra một kiểu dữ liệu mới không?
4. Vì sao gán `mode = 5` lại đọc ra 1?
5. `p->temp` tương đương với cách viết nào?
