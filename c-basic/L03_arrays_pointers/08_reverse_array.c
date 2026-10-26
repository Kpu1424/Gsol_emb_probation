/*
 * Ý chính: Đảo ngược mảng tại chỗ bằng 2 con trỏ: một ở đầu, một ở cuối, đổi chỗ rồi cùng tiến vào giữa.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 08_reverse_array.c -o 08_reverse_array
 * Quan sát: "Truoc reverse: 1 2 3 4 5" thành "Sau reverse: 5 4 3 2 1"; mảng của main bị đổi vì hàm nhận địa chỉ.
 */
#include <stdio.h>

void reverse(int *data, int n) {
  int *left = data;            /* trỏ phần tử đầu */
  int *right = data + n - 1;   /* trỏ phần tử cuối */

  while (left < right) {       /* gặp nhau ở giữa thì dừng */
    int tmp = *left;
    *left = *right;
    *right = tmp;
    left++;                    /* tiến 1 phần tử về bên phải */
    right--;                   /* lùi 1 phần tử về bên trái */
  }
}

void print_array(const char *label, int *data, int n) {
  printf("%s", label);
  for (int i = 0; i < n; i++) {
    printf(" %d", data[i]);
  }
  printf("\n");
}

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};

  print_array("Truoc reverse:", a, 5);
  reverse(a, 5);   /* phải truyền n riêng: trong hàm không tính được bằng sizeof */
  print_array("Sau reverse:  ", a, 5);
  return 0;
}
