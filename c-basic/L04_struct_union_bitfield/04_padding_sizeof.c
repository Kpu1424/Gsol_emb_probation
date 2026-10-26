/*
 * Ý chính: Compiler chèn byte trống (padding) để int nằm ở địa chỉ chia hết cho 4; đổi thứ tự thành viên thì sizeof có thể đổi.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 04_padding_sizeof.c -o 04_padding_sizeof
 * Quan sát: dữ liệu thật chỉ 6 byte, nhưng order_a = 12 byte, order_b = 8 byte (số liệu của gcc x86_64, máy khác có thể khác).
 */
#include <stdio.h>
#include <stddef.h>   /* offsetof: vị trí (byte thứ mấy) của thành viên tính từ đầu struct */

struct order_a { char a; int b; char c; };   /* nhỏ - lớn - nhỏ */
struct order_b { int b; char a; char c; };   /* lớn trước, nhỏ sau */

int main(void) {
  printf("Du lieu that: char + int + char = %zu byte\n", sizeof(char) + sizeof(int) + sizeof(char));

  printf("struct order_a {char a; int b; char c;}: sizeof = %zu\n", sizeof(struct order_a));
  printf("  offset a = %zu, b = %zu, c = %zu\n",
         offsetof(struct order_a, a), offsetof(struct order_a, b), offsetof(struct order_a, c));

  printf("struct order_b {int b; char a; char c;}: sizeof = %zu\n", sizeof(struct order_b));
  printf("  offset b = %zu, a = %zu, c = %zu\n",
         offsetof(struct order_b, b), offsetof(struct order_b, a), offsetof(struct order_b, c));
  return 0;
}
