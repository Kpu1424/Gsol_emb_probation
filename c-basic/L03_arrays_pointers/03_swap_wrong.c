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
