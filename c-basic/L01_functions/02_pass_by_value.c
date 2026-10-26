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
