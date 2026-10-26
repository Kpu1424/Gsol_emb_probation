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
