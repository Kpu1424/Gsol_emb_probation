# L03 - Mảng và con trỏ

Ý chính: Mỗi biến có một địa chỉ. Con trỏ là biến chứa địa chỉ, dùng `*` để đọc hoặc ghi vào ô nhớ nó trỏ tới. Tên mảng dùng như địa chỉ phần tử đầu, nên khi truyền mảng vào hàm thì hàm chỉ nhận một con trỏ: hàm sửa được mảng gốc, nhưng không biết mảng dài bao nhiêu.

| File | Ý minh họa | Lệnh chạy |
|------|------------|-----------|
| [01_address.c](01_address.c) | Toán tử `&` lấy địa chỉ | `make && ./build/01_address` |
| [02_pointer_deref.c](02_pointer_deref.c) | Con trỏ và `*` | `make && ./build/02_pointer_deref` |
| [03_swap_wrong.c](03_swap_wrong.c) | swap SAI (tham trị) | `make && ./build/03_swap_wrong` |
| [04_swap_right.c](04_swap_right.c) | swap ĐÚNG (con trỏ) | `make && ./build/04_swap_right` |
| [05_array_and_pointer.c](05_array_and_pointer.c) | `a[i]` chính là `*(a + i)` | `make && ./build/05_array_and_pointer` |
| [06_sizeof_in_function.c](06_sizeof_in_function.c) | `sizeof` mảng trong `main` so với trong hàm (**warning cố ý**) | `make && ./build/06_sizeof_in_function` |
| [07_string_char_array.c](07_string_char_array.c) | Chuỗi là mảng `char` có `'\0'` | `make && ./build/07_string_char_array` |
| [08_reverse_array.c](08_reverse_array.c) | Đảo mảng bằng con trỏ | `make && ./build/08_reverse_array` |

## 01_address.c
```c
/*
 * Ý chính: Mỗi biến nằm ở một địa chỉ trong bộ nhớ; toán tử & lấy địa chỉ đó.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 01_address.c -o 01_address
 * Quan sát: địa chỉ của a và b khác nhau (mỗi biến một chỗ riêng); địa chỉ sẽ khác trên máy bạn và mỗi lần chạy.
 */
#include <stdio.h>

int main(void) {
  int a = 25;
  int b = 30;

  printf("Gia tri cua a: %d\n", a);
  printf("Dia chi cua a: %p\n", (void *)&a);   /* %p in địa chỉ, chuẩn C yêu cầu ép sang (void *) */
  printf("Gia tri cua b: %d\n", b);
  printf("Dia chi cua b: %p\n", (void *)&b);
  printf("Moi bien int chiem sizeof(int) = %zu byte\n", sizeof(int));
  return 0;
}
```
Output (địa chỉ sẽ khác trên máy bạn và khác mỗi lần chạy):
```text
Gia tri cua a: 25
Dia chi cua a: 0x7ffcb406f080
Gia tri cua b: 30
Dia chi cua b: 0x7ffcb406f084
Moi bien int chiem sizeof(int) = 4 byte
```
Giải thích: `&a` cho biết biến `a` đang nằm ở đâu trong RAM. `a` và `b` là hai biến riêng nên có hai địa chỉ khác nhau. Mỗi `int` chiếm 4 byte. Địa chỉ đổi giữa các lần chạy vì Linux xếp vùng stack ở vị trí ngẫu nhiên (ASLR) để tăng bảo mật.

## 02_pointer_deref.c
```c
/*
 * Ý chính: Con trỏ là biến chứa địa chỉ; *p đọc/ghi vào ô nhớ mà p đang trỏ tới.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 02_pointer_deref.c -o 02_pointer_deref
 * Quan sát: "&x" và "p" in cùng một địa chỉ; sau "*p = 30" thì x cũng thành 30. Địa chỉ sẽ khác trên máy bạn.
 */
#include <stdio.h>

int main(void) {
  int x = 25;
  int *p = &x;   /* p giữ địa chỉ của x, nói gọn là "p trỏ tới x" */

  printf("Dia chi cua x (&x): %p\n", (void *)&x);
  printf("Gia tri cua p:      %p\n", (void *)p);
  printf("Doc qua con tro: *p = %d\n", *p);

  *p = 30;   /* ghi vào ô nhớ của x (không phải ghi vào p) */
  printf("Sau khi gan *p = 30: x = %d\n", x);
  return 0;
}
```
Output (địa chỉ sẽ khác trên máy bạn):
```text
Dia chi cua x (&x): 0x7ffc9e3d16fc
Gia tri cua p:      0x7ffc9e3d16fc
Doc qua con tro: *p = 25
Sau khi gan *p = 30: x = 30
```
Giải thích: `int *p = &x;` lưu địa chỉ của `x` vào `p`, nên giá trị của `p` trùng với `&x`. `*p` nghĩa là "ô nhớ mà `p` trỏ tới", tức chính là `x`. Vì vậy gán `*p = 30` làm `x` thành 30, dù dòng đó không viết tên `x`.

## 03_swap_wrong.c
```c
/*
 * Ý chính: (FILE SAI CỐ Ý) swap bằng tham trị chỉ đổi chỗ 2 bản sao, a và b trong main giữ nguyên.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 03_swap_wrong.c -o 03_swap_wrong
 * Quan sát: "Trong swap" đã đổi, nhưng "Sau swap: a = 1, b = 2". Đây là lỗi logic: gcc KHÔNG cảnh báo.
 */
#include <stdio.h>

void swap(int a, int b) {
  int tmp = a;
  a = b;
  b = tmp;
  printf("  Trong swap: a = %d, b = %d\n", a, b);   /* chỉ bản sao bị đổi */
}

int main(void) {
  int a = 1;
  int b = 2;

  printf("Truoc swap: a = %d, b = %d\n", a, b);
  swap(a, b);   /* gửi giá trị 1 và 2 vào hàm, hàm không biết a, b của main ở đâu */
  printf("Sau swap:   a = %d, b = %d\n", a, b);
  return 0;
}
```
Output:
```text
Truoc swap: a = 1, b = 2
  Trong swap: a = 2, b = 1
Sau swap:   a = 1, b = 2
```
Giải thích: Hàm nhận bản sao 1 và 2 (tham trị, L01). Bên trong hàm, hai bản sao được đổi chỗ, nên dòng "Trong swap" thấy đã đổi. Khi hàm kết thúc, các bản sao bị bỏ đi, còn `a`, `b` của `main` không bị đụng tới. gcc không cảnh báo vì code đúng cú pháp, chỉ sai ý đồ.

## 04_swap_right.c
```c
/*
 * Ý chính: Truyền ĐỊA CHỈ của biến vào hàm, hàm dùng * để sửa thẳng biến gốc của main.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 04_swap_right.c -o 04_swap_right
 * Quan sát: "Sau swap: a = 2, b = 1". So sánh với 03_swap_wrong.c.
 */
#include <stdio.h>

void swap(int *pa, int *pb) {
  int tmp = *pa;   /* đọc giá trị tại địa chỉ pa (chính là a của main) */
  *pa = *pb;
  *pb = tmp;
  printf("  Trong swap: *pa = %d, *pb = %d\n", *pa, *pb);
}

int main(void) {
  int a = 1;
  int b = 2;

  printf("Truoc swap: a = %d, b = %d\n", a, b);
  swap(&a, &b);   /* gửi địa chỉ: hàm biết a, b nằm ở đâu nên sửa được */
  printf("Sau swap:   a = %d, b = %d\n", a, b);
  return 0;
}
```
Output:
```text
Truoc swap: a = 1, b = 2
  Trong swap: *pa = 2, *pb = 1
Sau swap:   a = 2, b = 1
```
Giải thích: `swap(&a, &b)` gửi **địa chỉ** của `a` và `b`. Trong hàm, `*pa` chính là ô nhớ của `a` trong `main`, nên `*pa = *pb` ghi thẳng vào `a`. Đây cũng là cách để một hàm "trả về" nhiều kết quả: truyền địa chỉ của các biến nhận kết quả.

## 05_array_and_pointer.c
```c
/*
 * Ý chính: Tên mảng dùng như địa chỉ phần tử đầu; a[i] chính là *(a + i).
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 05_array_and_pointer.c -o 05_array_and_pointer
 * Quan sát: a[i] và *(p + i) bằng nhau; các địa chỉ liên tiếp cách nhau 4 byte = sizeof(int). Địa chỉ sẽ khác trên máy bạn.
 */
#include <stdio.h>

int main(void) {
  int a[3] = {25, 30, 35};
  int *p = a;   /* giống hệt int *p = &a[0]; */

  for (int i = 0; i < 3; i++) {
    /* p + i tiến i PHẦN TỬ (i * 4 byte với int), không phải i byte */
    printf("a[%d] = %d, *(p + %d) = %d, dia chi = %p\n", i, a[i], i, *(p + i), (void *)(p + i));
  }
  printf("a == &a[0] -> %d (1 la dung)\n", a == &a[0]);
  return 0;
}
```
Output (địa chỉ sẽ khác trên máy bạn):
```text
a[0] = 25, *(p + 0) = 25, dia chi = 0x7ffc44c0f8dc
a[1] = 30, *(p + 1) = 30, dia chi = 0x7ffc44c0f8e0
a[2] = 35, *(p + 2) = 35, dia chi = 0x7ffc44c0f8e4
a == &a[0] -> 1 (1 la dung)
```
Giải thích: Phần tử của mảng nằm sát nhau trong bộ nhớ, mỗi phần tử cách nhau 4 byte (đuôi địa chỉ tăng thêm 4). `p + 1` không cộng 1 byte mà tiến tới phần tử kế tiếp, tức cộng `sizeof(int)` byte. Vì vậy `*(p + i)` và `a[i]` là cùng một ô nhớ. Biểu thức `a == &a[0]` cho 1: tên mảng dùng như địa chỉ phần tử đầu.

## 06_sizeof_in_function.c
**Warning cố ý**: gcc cảnh báo `-Wsizeof-array-argument` để nhắc đúng lỗi mà file này minh họa. Warning thật được lưu ở đầu file output.
```c
/*
 * Ý chính: Mảng truyền vào hàm chỉ còn là con trỏ, nên sizeof trong hàm ra kích thước con trỏ, không phải cả mảng.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 06_sizeof_in_function.c -o 06_sizeof_in_function
 * Quan sát: trong main sizeof = 20 (5 phần tử x 4 byte), trong hàm sizeof = 8 (kích thước con trỏ trên máy 64-bit).
 * WARNING CỐ Ý: gcc báo -Wsizeof-array-argument ở dòng sizeof(data) để nhắc đúng lỗi này.
 */
#include <stdio.h>

/* Viết "int data[]" nhưng compiler hiểu là "int *data": hàm không nhận cả mảng */
void print_size(int data[]) {
  printf("Trong ham: sizeof(data) = %zu byte\n", sizeof(data));
}

int main(void) {
  int samples[5] = {25, 26, 27, 28, 29};

  printf("Trong main: sizeof(samples) = %zu byte\n", sizeof(samples));
  printf("Trong main: so phan tu = %zu\n", sizeof(samples) / sizeof(samples[0]));
  print_size(samples);
  /* Cách đúng: tính số phần tử ở main rồi truyền thêm tham số n (xem 08_reverse_array.c) */
  return 0;
}
```
Output (warning khi build + output khi chạy):
```text
=== Warning khi build (co y) ===
06_sizeof_in_function.c: In function ‘print_size’:
06_sizeof_in_function.c:11:56: warning: ‘sizeof’ on array function parameter ‘data’ will return size of ‘int *’ [-Wsizeof-array-argument]
   11 |   printf("Trong ham: sizeof(data) = %zu byte\n", sizeof(data));
      |                                                        ^
06_sizeof_in_function.c:10:21: note: declared here
   10 | void print_size(int data[]) {
      |                 ~~~~^~~~~~
=== Output khi chay ===
Trong main: sizeof(samples) = 20 byte
Trong main: so phan tu = 5
Trong ham: sizeof(data) = 8 byte
```
Giải thích: Trong `main`, `samples` là mảng thật nên `sizeof` = 5 × 4 = 20 byte. Khi truyền vào hàm, mảng chỉ còn là địa chỉ phần tử đầu. Tham số `int data[]` thực chất là `int *data`, nên `sizeof(data)` là kích thước con trỏ: 8 byte trên máy 64-bit. Vì vậy, khi truyền mảng vào hàm luôn phải truyền thêm số phần tử `n`, tính sẵn ở nơi khai báo mảng.

## 07_string_char_array.c
```c
/*
 * Ý chính: Chuỗi trong C là mảng char kết thúc bằng ký tự '\0'; strlen đếm ký tự trước '\0', sizeof đếm cả mảng.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 07_string_char_array.c -o 07_string_char_array
 * Quan sát: name[4] = 0 (ký tự '\0'); strlen = 4 nhưng sizeof = 5; sửa name[0] thì chuỗi đổi thành "WEMP".
 */
#include <stdio.h>
#include <string.h>   /* strlen */

int main(void) {
  char name[] = "TEMP";   /* 4 chữ + '\0' do compiler tự thêm = mảng 5 phần tử */

  for (int i = 0; i < 4; i++) {
    printf("name[%d] = '%c' (ma ASCII %d)\n", i, name[i], name[i]);
  }
  printf("name[4] = %d (ky tu '\\0' danh dau het chuoi)\n", name[4]);

  printf("strlen(name) = %zu\n", strlen(name));
  printf("sizeof(name) = %zu\n", sizeof(name));

  name[0] = 'W';   /* mảng char của mình nên sửa được từng ký tự */
  printf("Sau khi sua name[0]: %s\n", name);
  return 0;
}
```
Output:
```text
name[0] = 'T' (ma ASCII 84)
name[1] = 'E' (ma ASCII 69)
name[2] = 'M' (ma ASCII 77)
name[3] = 'P' (ma ASCII 80)
name[4] = 0 (ky tu '\0' danh dau het chuoi)
strlen(name) = 4
sizeof(name) = 5
Sau khi sua name[0]: WEMP
```
Giải thích: `"TEMP"` có 4 chữ, nhưng compiler cấp 5 ô và đặt `'\0'` (giá trị 0) vào ô cuối để đánh dấu hết chuỗi. `strlen` đếm ký tự cho tới khi gặp `'\0'` nên ra 4; `sizeof` đo cả mảng nên ra 5. `char name[]` là mảng của mình nên sửa được từng ký tự.

## 08_reverse_array.c
```c
/*
 * Ý chính: Đảo ngược mảng tại chỗ bằng 2 con trỏ: một ở đầu, một ở cuối, đổi chỗ rồi cùng tiến vào giữa.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 08_reverse_array.c -o 08_reverse_array
 * Quan sát: "Truoc reverse: 1 2 3 4 5" thành "Sau reverse: 5 4 3 2 1"; mảng của main bị đổi vì hàm nhận địa chỉ.
 */
#include <stdio.h>

void reverse(int *data, int n) {
  int *left = data;            /* trỏ phần tử đầu */
  int *right = data + n - 1;   /* trỏ phần tử cuối */

  while (left < right) {       /* gặp nhau ở giữa thì dừng */
    int tmp = *left;
    *left = *right;
    *right = tmp;
    left++;                    /* tiến 1 phần tử về bên phải */
    right--;                   /* lùi 1 phần tử về bên trái */
  }
}

void print_array(const char *label, int *data, int n) {
  printf("%s", label);
  for (int i = 0; i < n; i++) {
    printf(" %d", data[i]);
  }
  printf("\n");
}

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};

  print_array("Truoc reverse:", a, 5);
  reverse(a, 5);   /* phải truyền n riêng: trong hàm không tính được bằng sizeof */
  print_array("Sau reverse:  ", a, 5);
  return 0;
}
```
Output:
```text
Truoc reverse: 1 2 3 4 5
Sau reverse:   5 4 3 2 1
```
Giải thích: `left` bắt đầu ở phần tử đầu, `right` ở phần tử cuối (`data + n - 1`). Mỗi vòng đổi chỗ hai giá trị rồi cho hai con trỏ tiến vào giữa; khi `left` không còn nằm trước `right` thì dừng. Hàm nhận địa chỉ mảng nên sửa thẳng mảng của `main`. Số phần tử `n` phải truyền riêng (xem `06`).

## Lỗi hay gặp
- Dùng `sizeof(mang) / sizeof(mang[0])` bên trong hàm nhận mảng: kết quả sai vì đó là kích thước con trỏ (xem `06`).
- Truy cập ngoài mảng, ví dụ `a[5]` với mảng 5 phần tử (chỉ số hợp lệ là 0..4). C không kiểm tra, đây là hành vi không xác định.
- Dùng con trỏ chưa gán địa chỉ: `int *p; *p = 1;` có thể crash. Luôn gán `p = &bien` trước khi dùng `*p`.

## Tự kiểm tra
1. `p`, `*p` và `&x` khác nhau thế nào trong `02_pointer_deref.c`?
2. Vì sao `03_swap_wrong.c` không đổi được `a`, `b`, còn `04_swap_right.c` thì đổi được?
3. Vì sao `sizeof` trong hàm của `06` ra 8 chứ không phải 20?
4. Mảng `char s[] = "abc";` có bao nhiêu phần tử? `strlen(s)` bằng bao nhiêu?
5. Nếu `p` trỏ tới `a[0]` (mảng `int`) thì `p + 2` cách `p` bao nhiêu byte?
