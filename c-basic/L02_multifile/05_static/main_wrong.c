/*
 * Ý chính: (FILE SAI CỐ Ý) Cố dùng trực tiếp biến static count của counter.c bằng extern -> lỗi LINK.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 main_wrong.c counter.c -o static_wrong
 * Quan sát: compile qua được (đã có khai báo extern), nhưng linker báo "undefined reference to `count'".
 */
#include <stdio.h>

extern int count;   /* khai báo "count nằm ở file khác", nhưng counter.c đã giấu nó bằng static */

int main(void) {
  printf("Doc truc tiep count = %d\n", count);
  return 0;
}
