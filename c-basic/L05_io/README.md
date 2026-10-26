# L05 - Nhập/xuất bàn phím và file

Ý chính: `scanf` đọc theo định dạng và trả về số giá trị đọc thành công; với input sai, nó để lại dữ liệu rác trong bộ đệm. Cách an toàn là đọc cả dòng bằng `fgets` rồi đổi sang số bằng `strtol` có kiểm lỗi. Với file: `fopen` → đọc/ghi → `fclose`, luôn kiểm tra `fopen` có trả về `NULL` không. File nhị phân ghi y nguyên bộ nhớ, nên không mang sang máy khác được (không portable).

| File | Ý minh họa | Lệnh chạy |
|------|------------|-----------|
| [01_printf_scanf.c](01_printf_scanf.c) | `printf` / `scanf` cơ bản | `make && ./build/01_printf_scanf < 01_printf_scanf.in` |
| [02_scanf_wrong.c](02_scanf_wrong.c) | `scanf` khi gõ chữ (**file sai cố ý**) | `make && ./build/02_scanf_wrong < 02_scanf_wrong.in` |
| [03_fgets_strtol_right.c](03_fgets_strtol_right.c) | `fgets` + `strtol` có kiểm lỗi | `make && ./build/03_fgets_strtol_right < 03_fgets_strtol_right.in` |
| [04_write_csv.c](04_write_csv.c) | Ghi file CSV | `make && ./build/04_write_csv && cat sensor.csv` |
| [05_read_csv.c](05_read_csv.c) | Đọc file CSV | `make && ./build/05_read_csv` |
| [06_binary_file.c](06_binary_file.c) | File nhị phân và vì sao không portable | `make && ./build/06_binary_file` |

Dữ liệu nhập cho 01–03 nằm trong các file `.in`, nên `make` không phải chờ gõ tay. Muốn tự gõ thì chạy chương trình mà không có phần `< ....in`.

## 01_printf_scanf.c
```c
/*
 * Ý chính: scanf đọc theo định dạng và trả về SỐ giá trị đọc thành công; printf in theo định dạng.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 01_printf_scanf.c -o 01_printf_scanf && ./01_printf_scanf < 01_printf_scanf.in
 * Quan sát: input "25 3300" -> "scanf tra ve 2"; các dòng sau in lại 2 số theo vài định dạng khác nhau.
 */
#include <stdio.h>

int main(void) {
  int temp = 0;
  int voltage = 0;

  printf("Nhap nhiet do (C) va dien ap (mV), cach nhau dau cach:\n");
  int n = scanf("%d %d", &temp, &voltage);   /* scanf cần ĐỊA CHỈ của biến để ghi giá trị vào */
  printf("scanf tra ve %d (so gia tri doc duoc)\n", n);

  printf("temp    = %d C\n", temp);
  printf("voltage = %d mV\n", voltage);
  printf("temp rong 5 o:   [%5d] (can phai), [%-5d] (can trai)\n", temp, temp);
  printf("voltage dang hex: 0x%X\n", voltage);   /* hay dùng khi làm việc với thanh ghi */
  return 0;
}
```
Input (`01_printf_scanf.in`):
```text
25 3300
```
Output:
```text
Nhap nhiet do (C) va dien ap (mV), cach nhau dau cach:
scanf tra ve 2 (so gia tri doc duoc)
temp    = 25 C
voltage = 3300 mV
temp rong 5 o:   [   25] (can phai), [25   ] (can trai)
voltage dang hex: 0xCE4
```
Giải thích: `scanf("%d %d", &temp, &voltage)` cần **địa chỉ** của biến (L03) để ghi giá trị vào. Nó đọc được 2 số nên trả về 2. Trong `printf`, `%5d` dành tối thiểu 5 ô và căn phải, `%-5d` căn trái, `%X` in hệ 16 (3300 = 0xCE4).

## 02_scanf_wrong.c
```c
/*
 * Ý chính: (FILE SAI CỐ Ý) Gõ chữ khi scanf("%d") chờ số: scanf trả về 0, biến KHÔNG được gán, chữ vẫn nằm lại trong bộ đệm.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 02_scanf_wrong.c -o 02_scanf_wrong && ./02_scanf_wrong < 02_scanf_wrong.in
 * Quan sát: input "abc" rồi "25": cả 2 lần scanf đều trả về 0, temp vẫn là -1; số 25 không bao giờ được đọc.
 */
#include <stdio.h>

int main(void) {
  int temp = -1;   /* gán trước một giá trị dễ nhận ra; nếu không gán thì in ra sẽ là giá trị rác */

  printf("Nhap nhiet do (so nguyen):\n");
  int n = scanf("%d", &temp);
  printf("Lan 1: scanf tra ve %d, temp = %d\n", n, temp);

  /* "abc" chưa bị lấy ra khỏi bộ đệm, nên lần này scanf lại gặp 'a' và thất bại ngay */
  n = scanf("%d", &temp);
  printf("Lan 2: scanf tra ve %d, temp = %d\n", n, temp);
  return 0;
}
```
Input (`02_scanf_wrong.in`):
```text
abc
25
```
Output:
```text
Nhap nhiet do (so nguyen):
Lan 1: scanf tra ve 0, temp = -1
Lan 2: scanf tra ve 0, temp = -1
```
Giải thích: `%d` gặp chữ `a` thì dừng, không gán gì vào `temp` và trả về 0, nên `temp` vẫn là -1. Chữ `abc` **không** bị lấy ra khỏi bộ đệm, nên lần `scanf` thứ hai lại gặp đúng chữ `a` đó và thất bại tiếp; số 25 ở dòng sau không bao giờ được đọc. Nếu viết vòng lặp `while (scanf("%d", &x) != 1)` để "hỏi lại", chương trình sẽ lặp vô hạn.

## 03_fgets_strtol_right.c
```c
/*
 * Ý chính: Đọc cả dòng bằng fgets rồi đổi sang số bằng strtol, kiểm tra cả dòng có đúng là một số hợp lệ không.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 03_fgets_strtol_right.c -o 03_fgets_strtol_right && ./03_fgets_strtol_right < 03_fgets_strtol_right.in
 * Quan sát: "abc", "25abc", số quá lớn bị từ chối kèm lý do; "25" hợp lệ; hết input (EOF) thì dừng gọn.
 */
#include <stdio.h>
#include <stdlib.h>   /* strtol */
#include <errno.h>    /* errno, ERANGE */

int main(void) {
  char line[32];

  printf("Nhap nhiet do, moi dong mot so:\n");
  while (fgets(line, sizeof(line), stdin) != NULL) {   /* NULL = hết dữ liệu (EOF, hoặc Ctrl+D khi gõ tay) */
    printf("Doc duoc dong: %s", line);                  /* line đã có sẵn '\n' ở cuối */

    char *end;
    errno = 0;
    long value = strtol(line, &end, 10);   /* end trỏ tới ký tự đầu tiên KHÔNG thuộc về số */

    if (end == line) {
      printf("  Tu choi: khong co chu so nao\n");
    } else if (errno == ERANGE) {
      printf("  Tu choi: so qua lon, vuot gioi han cua long\n");
    } else if (*end != '\n' && *end != '\0') {
      printf("  Tu choi: co ky tu thua '%c' sau so\n", *end);
    } else {
      printf("  Hop le: nhiet do = %ld C\n", value);
    }
  }
  printf("Het du lieu (EOF), ket thuc.\n");
  return 0;
}
```
Input (`03_fgets_strtol_right.in`):
```text
abc
25abc
99999999999999999999
25
```
Output:
```text
Nhap nhiet do, moi dong mot so:
Doc duoc dong: abc
  Tu choi: khong co chu so nao
Doc duoc dong: 25abc
  Tu choi: co ky tu thua 'a' sau so
Doc duoc dong: 99999999999999999999
  Tu choi: so qua lon, vuot gioi han cua long
Doc duoc dong: 25
  Hop le: nhiet do = 25 C
Het du lieu (EOF), ket thuc.
```
Trường hợp không có input nào (EOF ngay từ đầu, giống nhấn Ctrl+D khi gõ tay):
```text
$ printf '' | ./build/03_fgets_strtol_right
Nhap nhiet do, moi dong mot so:
Het du lieu (EOF), ket thuc.
```
Giải thích: `fgets` lấy cả dòng ra khỏi bộ đệm, nên dòng sai không còn "kẹt lại" như ở `02`. `strtol` cho biết nó dừng ở đâu qua con trỏ `end`: `end == line` nghĩa là không có chữ số nào; `*end` khác `'\n'` nghĩa là còn ký tự thừa; `errno == ERANGE` nghĩa là số vượt giới hạn của `long`. Hết input thì `fgets` trả về `NULL`, vòng lặp dừng gọn.

## 04_write_csv.c
```c
/*
 * Ý chính: Ghi file văn bản: fopen("w") -> fprintf từng dòng -> fclose; luôn kiểm tra fopen có trả về NULL không.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 04_write_csv.c -o 04_write_csv && ./04_write_csv && cat sensor.csv
 * Quan sát: chương trình báo đã ghi 3 dòng; "cat sensor.csv" cho thấy 1 dòng tiêu đề + 3 dòng số.
 */
#include <stdio.h>

int main(void) {
  int temps[3] = {25, 26, 27};
  int volts[3] = {3300, 3290, 3310};

  FILE *f = fopen("sensor.csv", "w");   /* "w": tạo file mới; nếu đã có thì XÓA nội dung cũ */
  if (f == NULL) {
    perror("Khong mo duoc sensor.csv");  /* perror in kèm lý do hệ điều hành báo (vd: Permission denied) */
    return 1;
  }

  fprintf(f, "id,temp,voltage\n");       /* dòng tiêu đề */
  for (int i = 0; i < 3; i++) {
    fprintf(f, "%d,%d,%d\n", i, temps[i], volts[i]);   /* giống printf, nhưng ghi vào file f */
  }
  fclose(f);                             /* đóng file: đẩy phần còn trong bộ đệm xuống đĩa */

  printf("Da ghi 3 dong du lieu vao sensor.csv\n");
  return 0;
}
```
Output (dòng `$ cat sensor.csv` và nội dung bên dưới do Makefile chạy thêm):
```text
Da ghi 3 dong du lieu vao sensor.csv
$ cat sensor.csv
id,temp,voltage
0,25,3300
1,26,3290
2,27,3310
```
Giải thích: `fopen("sensor.csv", "w")` tạo file mới, hoặc xóa sạch nội dung nếu file đã có. Nếu không mở được (sai thư mục, không có quyền), `fopen` trả về `NULL`; ghi vào `NULL` sẽ crash, nên phải kiểm tra trước. `fprintf` giống `printf` nhưng ghi vào file. `fclose` đẩy phần dữ liệu còn trong bộ đệm xuống đĩa; quên `fclose` có thể làm mất dòng cuối.

## 05_read_csv.c
```c
/*
 * Ý chính: Đọc file văn bản từng dòng bằng fgets, tách các cột bằng sscanf; sscanf trả về số cột tách được.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 05_read_csv.c -o 05_read_csv && ./05_read_csv   (chạy 04_write_csv trước để có sensor.csv)
 * Quan sát: dòng tiêu đề bị bỏ qua (sscanf không tách được 3 số); 3 dòng dữ liệu được in lại; trung bình temp = 26.
 */
#include <stdio.h>

int main(void) {
  FILE *f = fopen("sensor.csv", "r");   /* "r": chỉ đọc, file phải có sẵn */
  if (f == NULL) {
    perror("Khong mo duoc sensor.csv (hay chay 04_write_csv truoc)");
    return 1;
  }

  char line[64];
  int id, temp, voltage;
  int count = 0, sum = 0;
  while (fgets(line, sizeof(line), f) != NULL) {   /* NULL = đã đọc hết file */
    if (sscanf(line, "%d,%d,%d", &id, &temp, &voltage) != 3) {
      printf("Bo qua dong: %s", line);              /* tiêu đề là chữ, không phải 3 số */
      continue;
    }
    printf("id = %d, temp = %d C, voltage = %d mV\n", id, temp, voltage);
    count++;
    sum += temp;
  }
  fclose(f);

  if (count > 0) {   /* tránh chia cho 0 khi file không có dòng dữ liệu nào */
    printf("Doc duoc %d dong du lieu, trung binh temp = %d C\n", count, sum / count);
  }
  return 0;
}
```
Output:
```text
Bo qua dong: id,temp,voltage
id = 0, temp = 25 C, voltage = 3300 mV
id = 1, temp = 26 C, voltage = 3290 mV
id = 2, temp = 27 C, voltage = 3310 mV
Doc duoc 3 dong du lieu, trung binh temp = 26 C
```
Giải thích: `fgets` đọc từng dòng tới khi hết file thì trả về `NULL`. `sscanf` tách chuỗi giống `scanf` và trả về số giá trị tách được. Dòng tiêu đề `id,temp,voltage` không có số nào nên `sscanf` trả về 0 và dòng đó bị bỏ qua. Trung bình (25 + 26 + 27) / 3 = 26.

## 06_binary_file.c
```c
/*
 * Ý chính: fwrite ghi nguyên các byte của struct trong bộ nhớ, gồm cả padding và thứ tự byte của máy này -> file không portable.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 06_binary_file.c -o 06_binary_file && ./06_binary_file
 * Quan sát: file 8 byte dù dữ liệu chỉ 5 byte; 3 byte "00" sau id là padding; 3300 = 0x00000CE4 được ghi ngược thành E4 0C 00 00.
 */
#include <stdio.h>
#include <string.h>   /* memset */

struct record {
  char id;       /* 1 byte, sau nó compiler chèn 3 byte padding (xem L04 04_padding_sizeof.c) */
  int voltage;   /* 4 byte */
};

int main(void) {
  struct record r;
  memset(&r, 0, sizeof(r));   /* xóa cả byte padding về 0, nếu không chúng chứa giá trị rác bất kỳ */
  r.id = 1;
  r.voltage = 3300;

  FILE *f = fopen("record.bin", "wb");   /* "b": nhị phân (Linux không khác, Windows có khác) */
  if (f == NULL) { perror("Khong mo duoc record.bin"); return 1; }
  fwrite(&r, sizeof(r), 1, f);            /* ghi 1 khối sizeof(r) byte, y nguyên như trong RAM */
  fclose(f);

  unsigned char bytes[16];
  f = fopen("record.bin", "rb");
  if (f == NULL) { perror("Khong mo duoc record.bin"); return 1; }
  size_t n = fread(bytes, 1, sizeof(bytes), f);   /* đọc lại từng byte, trả về số byte đọc được */
  fclose(f);

  printf("sizeof(struct record) = %zu, du lieu that = 1 + 4 = 5 byte\n", sizeof(struct record));
  printf("record.bin co %zu byte:", n);
  for (size_t i = 0; i < n; i++) {
    printf(" %02X", bytes[i]);
  }
  printf("\n");
  return 0;
}
```
Output (giá trị của gcc trên x86_64; máy khác có thể khác):
```text
sizeof(struct record) = 8, du lieu that = 1 + 4 = 5 byte
record.bin co 8 byte: 01 00 00 00 E4 0C 00 00
```
Giải thích: `fwrite` chép y nguyên 8 byte của `r` trong RAM ra file: `01` là `id`, ba byte `00` là padding (L04, `04_padding_sizeof.c`), và `E4 0C 00 00` là 3300 = 0x00000CE4 xếp theo kiểu little-endian (L04, `05_union.c`). Một máy big-endian, hoặc compiler có quy tắc padding khác, đọc file này sẽ ra giá trị sai, nên file `.bin` kiểu này **không portable**. Muốn portable thì phải tự quy định định dạng từng byte. `memset` đưa padding về 0; nếu không có nó, ba byte đó là giá trị rác, mỗi lần chạy có thể khác.

## Lỗi hay gặp
- Dùng `scanf("%d")` để đọc input người dùng mà không kiểm tra giá trị trả về: chương trình dùng biến chưa được gán, hoặc lặp vô hạn (xem `02`).
- Không kiểm tra `fopen` trả về `NULL`, rồi ghi/đọc luôn: crash khi file không mở được.
- Ghi struct ra file nhị phân bằng `fwrite` rồi đọc trên máy khác: sai do padding và endianness (xem `06`).

## Tự kiểm tra
1. Giá trị trả về của `scanf` cho biết điều gì? Vì sao ở `02` cả hai lần đều trả về 0?
2. Vì sao `fgets` + `strtol` an toàn hơn `scanf`? Con trỏ `end` của `strtol` dùng để làm gì?
3. Mở file bằng `"w"` và bằng `"r"` khác nhau thế nào? Nếu mở `"r"` một file không tồn tại thì sao?
4. Vì sao `record.bin` có 8 byte chứ không phải 5?
5. Vì sao file `.bin` ghi bằng `fwrite` có thể bị đọc sai trên máy khác?
