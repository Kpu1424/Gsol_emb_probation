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
