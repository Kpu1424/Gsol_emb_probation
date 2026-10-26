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
