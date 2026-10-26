/*
 * Ý chính: Mỗi biến nằm ở một địa chỉ trong bộ nhớ; toán tử & lấy địa chỉ đó.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 01_address.c -o 01_address
 * Quan sát: địa chỉ của a và b khác nhau (mỗi biến một chỗ riêng); địa chỉ sẽ khác trên máy bạn và mỗi lần chạy.
 */
#include <stdio.h>

int main(void) {
  int a = 25;
  int b = 30;

  printf("Gia tri cua a: %d\n", a);
  printf("Dia chi cua a: %p\n", (void *)&a);   /* %p in địa chỉ, chuẩn C yêu cầu ép sang (void *) */
  printf("Gia tri cua b: %d\n", b);
  printf("Dia chi cua b: %p\n", (void *)&b);
  printf("Moi bien int chiem sizeof(int) = %zu byte\n", sizeof(int));
  return 0;
}
