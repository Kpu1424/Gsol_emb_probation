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
