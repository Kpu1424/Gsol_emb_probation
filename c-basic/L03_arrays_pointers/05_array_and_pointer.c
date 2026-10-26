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
