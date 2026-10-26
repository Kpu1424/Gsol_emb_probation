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
