/*
 * Ý chính: (FILE SAI CỐ Ý) Gõ chữ khi scanf("%d") chờ số: scanf trả về 0, biến KHÔNG được gán, chữ vẫn nằm lại trong bộ đệm.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 02_scanf_wrong.c -o 02_scanf_wrong && ./02_scanf_wrong < 02_scanf_wrong.in
 * Quan sát: input "abc" rồi "25": cả 2 lần scanf đều trả về 0, temp vẫn là -1; số 25 không bao giờ được đọc.
 */
#include <stdio.h>

int main(void) {
  int temp = -1;   /* gán trước một giá trị dễ nhận ra; nếu không gán thì in ra sẽ là giá trị rác */

  printf("Nhap nhiet do (so nguyen):\n");
  int n = scanf("%d", &temp);
  printf("Lan 1: scanf tra ve %d, temp = %d\n", n, temp);

  /* "abc" chưa bị lấy ra khỏi bộ đệm, nên lần này scanf lại gặp 'a' và thất bại ngay */
  n = scanf("%d", &temp);
  printf("Lan 2: scanf tra ve %d, temp = %d\n", n, temp);
  return 0;
}
