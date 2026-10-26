/*
 * Ý chính: scanf đọc theo định dạng và trả về SỐ giá trị đọc thành công; printf in theo định dạng.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 01_printf_scanf.c -o 01_printf_scanf && ./01_printf_scanf < 01_printf_scanf.in
 * Quan sát: input "25 3300" -> "scanf tra ve 2"; các dòng sau in lại 2 số theo vài định dạng khác nhau.
 */
#include <stdio.h>

int main(void) {
  int temp = 0;
  int voltage = 0;

  printf("Nhap nhiet do (C) va dien ap (mV), cach nhau dau cach:\n");
  int n = scanf("%d %d", &temp, &voltage);   /* scanf cần ĐỊA CHỈ của biến để ghi giá trị vào */
  printf("scanf tra ve %d (so gia tri doc duoc)\n", n);

  printf("temp    = %d C\n", temp);
  printf("voltage = %d mV\n", voltage);
  printf("temp rong 5 o:   [%5d] (can phai), [%-5d] (can trai)\n", temp, temp);
  printf("voltage dang hex: 0x%X\n", voltage);   /* hay dùng khi làm việc với thanh ghi */
  return 0;
}
