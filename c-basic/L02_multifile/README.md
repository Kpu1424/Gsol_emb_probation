# L02 - Tách chương trình thành nhiều file

Ý chính: File `.h` chứa **khai báo** ("có hàm/biến này"), file `.c` chứa **định nghĩa** ("thân hàm, bộ nhớ của biến"). gcc build qua 2 bước: **compile** từng file `.c` thành `.o` (lúc này chỉ cần khai báo), rồi **link** các `.o` thành chương trình (lúc này phải tìm được định nghĩa). `extern` dùng chung một biến giữa nhiều file; `static` giấu biến bên trong một file.

| File | Ý minh họa | Lệnh chạy |
|------|------------|-----------|
| [01_basic/](01_basic/) `sensor.h`, `sensor.c`, `main.c` | Tách file `.h` / `.c` | `make && ./build/01_basic` |
| [02_compile_error/main_wrong.c](02_compile_error/main_wrong.c) | Lỗi compile: thiếu khai báo (**file sai cố ý**) | `make compile_error` |
| (không có code mới, dùng lại `01_basic/main.c`) | Lỗi link: thiếu định nghĩa (**cố ý**) | `make link_error` |
| [04_extern/](04_extern/) `config.h`, `config.c`, `main.c` | `extern`: nhiều file dùng chung một biến | `make && ./build/04_extern` |
| [05_static/](05_static/) `counter.h`, `counter.c`, `main_right.c` | `static` ở mức file: dùng qua hàm | `make && ./build/05_static_right` |
| [05_static/main_wrong.c](05_static/main_wrong.c) | Dùng trực tiếp biến `static` thì lỗi link (**file sai cố ý**) | `make` (xem `output/05_static_wrong.txt`) |

Với các ví dụ lỗi, Makefile chỉ báo thành công khi gcc **thất bại** đúng như mong đợi. Thông báo lỗi thật được lưu vào `output/`, dòng đầu là lệnh đã chạy.

## 01_basic
`01_basic/sensor.h`:
```c
/*
 * Ý chính: Header (.h) chỉ chứa KHAI BÁO: "có hàm này", không chứa thân hàm.
 * Build:   (xem main.c)
 * Quan sát: file .c nào muốn gọi sensor_read_temp thì #include "sensor.h".
 */
#ifndef SENSOR_H   /* include guard: nếu một file .c lỡ include 2 lần thì lần sau bị bỏ qua */
#define SENSOR_H

int sensor_read_temp(void);

#endif
```
`01_basic/sensor.c`:
```c
/*
 * Ý chính: File .c chứa ĐỊNH NGHĨA (thân hàm) cho những gì sensor.h đã khai báo.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 -c sensor.c -o sensor.o   (chỉ compile, chưa link)
 * Quan sát: file này không chạy riêng được vì không có main; xem main.c.
 */
#include "sensor.h"   /* include header của chính mình: compiler kiểm tra định nghĩa khớp khai báo */

int sensor_read_temp(void) {
  return 25;   /* giả lập cảm biến: luôn đọc được 25 độ C */
}
```
`01_basic/main.c`:
```c
/*
 * Ý chính: main.c chỉ cần #include "sensor.h" để gọi hàm định nghĩa ở sensor.c; bước link nối 2 file lại.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 main.c sensor.c -o sensor_app
 * Quan sát: "Nhiet do doc tu sensor.c: 25 C".
 */
#include <stdio.h>
#include "sensor.h"   /* dấu "" : tìm file trong thư mục hiện tại trước */

int main(void) {
  int t = sensor_read_temp();   /* compiler chỉ cần khai báo; thân hàm do linker tìm trong sensor.o */
  printf("Nhiet do doc tu sensor.c: %d C\n", t);
  return 0;
}
```
Output:
```text
Nhiet do doc tu sensor.c: 25 C
```
Giải thích: Khi compile `main.c`, gcc chỉ cần khai báo `int sensor_read_temp(void);` lấy từ `sensor.h` để kiểm tra lời gọi hàm có đúng không. Thân hàm nằm trong `sensor.o`, và đến bước link mới được nối vào. Makefile làm đúng 3 lệnh: `gcc -c main.c`, `gcc -c sensor.c`, rồi `gcc main.o sensor.o -o ...`. Include guard (`#ifndef SENSOR_H`) giúp header không bị xử lý 2 lần nếu lỡ include 2 lần.

## 02_compile_error
`02_compile_error/main_wrong.c`:
```c
/*
 * Ý chính: (FILE SAI CỐ Ý) Dùng biến temp_limit chưa hề được khai báo -> gcc dừng ở bước COMPILE, chưa tới bước link.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 -c main_wrong.c -o main_wrong.o
 * Quan sát: "error: 'temp_limit' undeclared": lỗi compile luôn chỉ ra tên file .c, số dòng và cột.
 */
#include <stdio.h>
#include "../01_basic/sensor.h"

int main(void) {
  int t = sensor_read_temp();

  /* temp_limit không được khai báo ở đâu cả (ví dụ: quên #include header có khai báo nó) */
  if (t > temp_limit) {
    printf("Qua nhiet\n");
  }
  return 0;
}
```
Output (lỗi thật của gcc):
```text
$ gcc -std=c11 -Wall -Wextra -g -O0 -c 02_compile_error/main_wrong.c -o build/main_wrong.o
02_compile_error/main_wrong.c: In function ‘main’:
02_compile_error/main_wrong.c:13:11: error: ‘temp_limit’ undeclared (first use in this function)
   13 |   if (t > temp_limit) {
      |           ^~~~~~~~~~
02_compile_error/main_wrong.c:13:11: note: each undeclared identifier is reported only once for each function it appears in
```
Giải thích: Ở bước compile, gcc gặp tên `temp_limit` mà phía trên không có khai báo nào nên dừng lại ngay, không tạo ra file `.o` nào. Dấu hiệu nhận biết lỗi compile: thông báo bắt đầu bằng `tên_file.c:dòng:cột: error:`. Lưu ý: nếu quên khai báo **hàm** (ví dụ quên `#include "sensor.h"` rồi gọi `sensor_read_temp()`), gcc 11 chỉ báo **warning** `implicit declaration of function`; từ gcc 14 trường hợp đó mới thành **error**.

## 03_link_error
Lệnh `make link_error` link `main.o` một mình, cố ý quên `sensor.o`.

Output (lỗi thật; đường dẫn tuyệt đối sẽ khác trên máy bạn):
```text
$ gcc build/obj/01_basic/main.o -o build/03_link_error
/usr/bin/ld: build/obj/01_basic/main.o: in function `main':
/home/emb01/tuan/emb/emb_probation/c-basic/L02_multifile/01_basic/main.c:10: undefined reference to `sensor_read_temp'
collect2: error: ld returned 1 exit status
```
Giải thích: `main.c` compile được vì đã có khai báo từ `sensor.h`. Đến bước link, linker (`/usr/bin/ld`) cần thân hàm `sensor_read_temp` nhưng không có trong file `.o` nào được đưa vào, nên báo `undefined reference`. Dấu hiệu nhận biết lỗi link: thông báo đến từ `ld`/`collect2`, nhắc tới file `.o` và tên hàm hoặc biến, không có số cột.

## 04_extern
`04_extern/config.h`:
```c
/*
 * Ý chính: extern = KHAI BÁO biến toàn cục: "biến này có thật, nằm ở một file .c khác".
 * Build:   (xem main.c)
 * Quan sát: dòng extern không cấp bộ nhớ; bộ nhớ được cấp ở config.c.
 */
#ifndef CONFIG_H
#define CONFIG_H

extern int g_temp_limit;   /* khai báo, KHÔNG có "= 40" */
void config_set_limit(int limit);

#endif
```
`04_extern/config.c`:
```c
/*
 * Ý chính: ĐỊNH NGHĨA biến toàn cục g_temp_limit: cấp bộ nhớ, chỉ được có đúng 1 lần trong cả chương trình.
 * Build:   (xem main.c)
 * Quan sát: main.c đọc được biến này nhờ khai báo extern trong config.h.
 */
#include "config.h"

int g_temp_limit = 40;   /* định nghĩa: nếu đặt dòng này trong .h sẽ bị "multiple definition" khi nhiều .c include */

void config_set_limit(int limit) {
  g_temp_limit = limit;
}
```
`04_extern/main.c`:
```c
/*
 * Ý chính: Nhờ extern, main.c và config.c cùng dùng MỘT biến g_temp_limit duy nhất.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 main.c config.c -o extern_app
 * Quan sát: config.c đổi giá trị thành 50, main.c đọc lại thấy 50 ngay: không có bản sao nào.
 */
#include <stdio.h>
#include "config.h"

int main(void) {
  printf("main.c doc g_temp_limit = %d\n", g_temp_limit);

  config_set_limit(50);   /* hàm trong config.c sửa biến */
  printf("Sau config_set_limit(50), main.c doc lai g_temp_limit = %d\n", g_temp_limit);
  return 0;
}
```
Output:
```text
main.c doc g_temp_limit = 40
Sau config_set_limit(50), main.c doc lai g_temp_limit = 50
```
Giải thích: `extern int g_temp_limit;` trong `config.h` chỉ là khai báo, không cấp bộ nhớ. Bộ nhớ thật được cấp đúng một lần trong `config.c` (`int g_temp_limit = 40;`). `main.c` và `config.c` vì vậy cùng trỏ tới **một** biến: `config.c` đổi thành 50 thì `main.c` đọc lại thấy 50 ngay.

## 05_static (đúng)
`05_static/counter.h`:
```c
/*
 * Ý chính: Header chỉ công khai 2 hàm; biến count bên trong counter.c KHÔNG được khai báo ở đây.
 * Build:   (xem main_right.c)
 * Quan sát: file khác chỉ đụng tới count qua 2 hàm này.
 */
#ifndef COUNTER_H
#define COUNTER_H

void counter_increment(void);
int counter_get(void);

#endif
```
`05_static/counter.c`:
```c
/*
 * Ý chính: static ở mức file: biến count chỉ nhìn thấy trong counter.c, file khác không truy cập trực tiếp được.
 * Build:   (xem main_right.c, main_wrong.c)
 * Quan sát: main_right.c dùng được count qua hàm; main_wrong.c dùng trực tiếp thì lỗi link.
 */
#include "counter.h"

static int count = 0;   /* static: tên count không được đưa ra cho linker, nên file khác không "thấy" nó */

void counter_increment(void) {
  count++;
}

int counter_get(void) {
  return count;
}
```
`05_static/main_right.c`:
```c
/*
 * Ý chính: Muốn đọc/sửa biến static của counter.c thì phải đi qua các hàm mà counter.c cung cấp.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 main_right.c counter.c -o static_right
 * Quan sát: "counter_get() = 3" sau 3 lần gọi counter_increment().
 */
#include <stdio.h>
#include "counter.h"

int main(void) {
  counter_increment();
  counter_increment();
  counter_increment();
  printf("Sau 3 lan goi counter_increment(): counter_get() = %d\n", counter_get());
  return 0;
}
```
Output:
```text
Sau 3 lan goi counter_increment(): counter_get() = 3
```
Giải thích: `count` là `static` nên chỉ code trong `counter.c` thấy nó. `main_right.c` muốn tăng hoặc đọc `count` thì phải gọi `counter_increment()` và `counter_get()`. Đây là cách "đóng gói" trong C: file khác không thể vô tình sửa `count`.

## 05_static (sai)
`05_static/main_wrong.c`:
```c
/*
 * Ý chính: (FILE SAI CỐ Ý) Cố dùng trực tiếp biến static count của counter.c bằng extern -> lỗi LINK.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 main_wrong.c counter.c -o static_wrong
 * Quan sát: compile qua được (đã có khai báo extern), nhưng linker báo "undefined reference to `count'".
 */
#include <stdio.h>

extern int count;   /* khai báo "count nằm ở file khác", nhưng counter.c đã giấu nó bằng static */

int main(void) {
  printf("Doc truc tiep count = %d\n", count);
  return 0;
}
```
Output (lỗi thật; đường dẫn tuyệt đối sẽ khác trên máy bạn):
```text
$ gcc build/obj/05_static/main_wrong.o build/obj/05_static/counter.o -o build/05_static_wrong
/usr/bin/ld: build/obj/05_static/main_wrong.o: warning: relocation against `count' in read-only section `.text'
/usr/bin/ld: build/obj/05_static/main_wrong.o: in function `main':
/home/emb01/tuan/emb/emb_probation/c-basic/L02_multifile/05_static/main_wrong.c:11: undefined reference to `count'
/usr/bin/ld: warning: creating DT_TEXTREL in a PIE
collect2: error: ld returned 1 exit status
```
Giải thích: `main_wrong.c` compile được vì có dòng `extern int count;`. Nhưng `static` đã giấu `count` khỏi linker, nên ở bước link không tìm thấy biến nào tên `count` và báo `undefined reference to 'count'`. Hai dòng `warning: relocation ...` và `DT_TEXTREL` là hệ quả phụ của chính lỗi này; dòng cần đọc là `undefined reference`.

## Lỗi hay gặp
- Đặt **định nghĩa** biến (`int g = 0;`) trong file `.h`, rồi nhiều file `.c` cùng include header đó: linker báo `multiple definition`. Trong `.h` chỉ đặt `extern int g;`.
- Không phân biệt lỗi compile và lỗi link: lỗi compile có `file.c:dòng:cột: error`, còn lỗi link có `undefined reference` từ `ld`.
- Build chỉ một file `gcc main.c` mà quên các file `.c` khác: lỗi link (xem `03`).

## Tự kiểm tra
1. File `.h` nên chứa gì, file `.c` nên chứa gì?
2. Nhìn vào một thông báo lỗi, làm sao biết đó là lỗi compile hay lỗi link?
3. `extern int g;` và `int g = 0;` khác nhau thế nào?
4. Vì sao `05_static/main_wrong.c` compile được mà vẫn lỗi ở bước link?
5. Gõ tay 3 lệnh gcc để build `01_basic` (không dùng make).
