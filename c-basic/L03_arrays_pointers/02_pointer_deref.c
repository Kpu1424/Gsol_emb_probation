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
