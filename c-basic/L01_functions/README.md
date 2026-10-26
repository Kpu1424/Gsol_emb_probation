# L01 - Hàm và cấu trúc chương trình

Ý chính: Chương trình C bắt đầu chạy từ `main`. Hàm phải được khai báo trước khi gọi, nhận tham số bằng cách **chép giá trị** (tham trị) và gửi kết quả về bằng `return`. Giá trị `return` của `main` là mã thoát, shell đọc được bằng `echo $?`.

| File | Ý minh họa | Lệnh chạy |
|------|------------|-----------|
| [01_function_basic.c](01_function_basic.c) | Khai báo, định nghĩa, gọi hàm | `make && ./build/01_function_basic` |
| [02_pass_by_value.c](02_pass_by_value.c) | Tham trị: sửa tham số không đổi biến gốc | `make && ./build/02_pass_by_value` |
| [03_return_value.c](03_return_value.c) | Giá trị trả về của hàm | `make && ./build/03_return_value` |
| [04_main_return_code.c](04_main_return_code.c) | Mã thoát của `main` | `make && ./build/04_main_return_code; echo $?` |

## 01_function_basic.c
```c
/*
 * Ý chính: Một hàm có 3 phần: khai báo (prototype), định nghĩa (thân hàm) và lời gọi hàm.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 01_function_basic.c -o 01_function_basic
 * Quan sát: các dòng "Trong ham" nằm giữa "Truoc" và "Sau": gọi hàm = nhảy vào thân hàm rồi quay lại.
 */
#include <stdio.h>

/* Khai báo: cho compiler biết trước tên hàm, kiểu tham số, kiểu trả về.
   Nhờ vậy main gọi được hàm dù thân hàm nằm ở cuối file. */
void print_temp(int celsius);

int main(void) {
  printf("Truoc khi goi ham\n");
  print_temp(25);   /* giá trị 25 được chép vào tham số celsius */
  print_temp(30);   /* cùng một hàm, dùng lại với giá trị khác */
  printf("Sau khi goi ham\n");
  return 0;
}

/* Định nghĩa: phần thân, nơi công việc thật sự được làm */
void print_temp(int celsius) {
  printf("  Trong ham print_temp: nhiet do = %d C\n", celsius);
}
```
Output:
```text
Truoc khi goi ham
  Trong ham print_temp: nhiet do = 25 C
  Trong ham print_temp: nhiet do = 30 C
Sau khi goi ham
```
Giải thích: Dòng `void print_temp(int celsius);` ở đầu file là **khai báo**, báo cho compiler biết hàm này tồn tại, nên `main` gọi được dù thân hàm nằm ở cuối file. Mỗi lần gọi `print_temp(25)`, chương trình chép 25 vào `celsius`, chạy thân hàm, rồi quay lại đúng chỗ đã gọi. Vì vậy hai dòng "Trong ham" nằm giữa "Truoc" và "Sau".

## 02_pass_by_value.c
```c
/*
 * Ý chính: C truyền tham trị: hàm nhận BẢN SAO giá trị, sửa tham số không làm đổi biến gốc.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 02_pass_by_value.c -o 02_pass_by_value
 * Quan sát: "Trong ham, sau khi gan: value = 0" nhưng "Sau khi goi ham: x = 25".
 */
#include <stdio.h>

void set_to_zero(int value) {
  printf("  Trong ham, truoc khi gan: value = %d\n", value);
  value = 0;   /* chỉ đổi bản sao nằm trong hàm, x của main không liên quan */
  printf("  Trong ham, sau khi gan:   value = %d\n", value);
}

int main(void) {
  int x = 25;

  printf("Truoc khi goi ham: x = %d\n", x);
  set_to_zero(x);   /* gửi GIÁ TRỊ 25 vào hàm, không gửi chính biến x */
  printf("Sau khi goi ham:   x = %d\n", x);
  return 0;
}
```
Output:
```text
Truoc khi goi ham: x = 25
  Trong ham, truoc khi gan: value = 25
  Trong ham, sau khi gan:   value = 0
Sau khi goi ham:   x = 25
```
Giải thích: Khi gọi `set_to_zero(x)`, C tạo một biến mới tên `value` và chép giá trị 25 vào đó. Lệnh `value = 0` chỉ sửa bản sao này, nên trong hàm thấy 0 nhưng `x` của `main` vẫn là 25. Muốn hàm sửa được biến gốc thì phải truyền địa chỉ (L03, `04_swap_right.c`).

## 03_return_value.c
```c
/*
 * Ý chính: return gửi một giá trị từ hàm về chỗ gọi; chỗ gọi có thể lưu vào biến hoặc dùng ngay.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 03_return_value.c -o 03_return_value
 * Quan sát: "max(25, 30) = 30", "is_overheat(45) = 1", "is_overheat(25) = 0".
 */
#include <stdio.h>

int max(int a, int b) {
  if (a > b) {
    return a;   /* return kết thúc hàm ngay, dòng bên dưới không chạy nữa */
  }
  return b;
}

/* Cách truyền thống của C: dùng int làm kiểu đúng/sai, 1 = đúng, 0 = sai */
int is_overheat(int celsius) {
  return celsius > 40;   /* phép so sánh tự có giá trị 1 hoặc 0 */
}

int main(void) {
  int m = max(25, 30);   /* cách 1: lưu giá trị trả về vào biến */
  printf("max(25, 30) = %d\n", m);

  printf("is_overheat(45) = %d\n", is_overheat(45));   /* cách 2: dùng ngay trong printf */
  printf("is_overheat(25) = %d\n", is_overheat(25));

  if (is_overheat(45)) {   /* cách 3: dùng làm điều kiện */
    printf("45 C la qua nhiet\n");
  }
  return 0;
}
```
Output:
```text
max(25, 30) = 30
is_overheat(45) = 1
is_overheat(25) = 0
45 C la qua nhiet
```
Giải thích: `return` vừa gửi giá trị về vừa kết thúc hàm. Với `max(25, 30)`, điều kiện `25 > 30` sai nên hàm đi tới `return b;` và trả về 30. `celsius > 40` là biểu thức có giá trị 1 (đúng) hoặc 0 (sai), nên `is_overheat` trả thẳng kết quả so sánh. Giá trị trả về dùng như một số bình thường: gán vào biến, đưa vào `printf`, hoặc làm điều kiện của `if`.

## 04_main_return_code.c
```c
/*
 * Ý chính: Giá trị return của main là mã thoát gửi cho hệ điều hành: 0 = thành công, khác 0 = có lỗi.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 04_main_return_code.c -o 04_main_return_code
 * Quan sát: chạy "./04_main_return_code; echo $?" thì dòng cuối là 1 (do nhiệt độ vượt ngưỡng).
 */
#include <stdio.h>

int main(void) {
  int celsius = 45;
  int limit = 40;

  printf("Nhiet do = %d C, nguong = %d C\n", celsius, limit);
  if (celsius > limit) {
    printf("Qua nhiet -> main tra ve 1\n");
    return 1;   /* shell/script đọc được số này (echo $?) để biết chương trình báo lỗi */
  }
  printf("Binh thuong -> main tra ve 0\n");
  return 0;
}
```
Output (2 dòng cuối là lệnh `echo $?` do Makefile chạy thêm, không phải do chương trình in):
```text
Nhiet do = 45 C, nguong = 40 C
Qua nhiet -> main tra ve 1
$ echo $?
1
```
Giải thích: Vì 45 > 40, `main` chạy tới `return 1;` và chương trình kết thúc với mã 1. Shell lưu mã thoát của lệnh vừa chạy vào biến `$?`, nên `echo $?` in ra 1. Quy ước: 0 là thành công, khác 0 là lỗi. Script và Makefile dựa vào mã này để biết lệnh có chạy thành công hay không. Ví dụ, Makefile sẽ dừng nếu một lệnh trả về khác 0.

## Lỗi hay gặp
- Gọi hàm khi phía trên chưa có khai báo: gcc 11 chỉ cảnh báo `implicit declaration of function`, chương trình vẫn build được nhưng có thể chạy sai. Luôn đặt prototype ở đầu file hoặc trong file `.h`.
- Quên `return` trong hàm không phải `void`: giá trị trả về là rác. `-Wall` sẽ cảnh báo `control reaches end of non-void function`.
- Nghĩ rằng sửa tham số trong hàm thì biến của người gọi cũng đổi (xem `02_pass_by_value.c`).

## Tự kiểm tra
1. Khai báo hàm và định nghĩa hàm khác nhau thế nào? Nếu xóa dòng khai báo trong `01_function_basic.c` thì gcc báo gì?
2. Vì sao trong `02_pass_by_value.c`, `x` vẫn là 25 sau khi gọi hàm?
3. Lệnh `return` có hai tác dụng nào?
4. Sau khi chạy `04_main_return_code`, `echo $?` in ra gì? Nếu đổi `celsius` thành 30 thì in ra gì?
