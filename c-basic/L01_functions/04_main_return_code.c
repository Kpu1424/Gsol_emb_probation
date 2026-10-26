/*
 * Ý chính: Giá trị return của main là mã thoát gửi cho hệ điều hành: 0 = thành công, khác 0 = có lỗi.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 04_main_return_code.c -o 04_main_return_code
 * Quan sát: chạy "./04_main_return_code; echo $?" thì dòng cuối là 1 (do nhiệt độ vượt ngưỡng).
 */
#include <stdio.h>

int main(void) {
  int celsius = 45;
  int limit = 40;

  printf("Nhiet do = %d C, nguong = %d C\n", celsius, limit);
  if (celsius > limit) {
    printf("Qua nhiet -> main tra ve 1\n");
    return 1;   /* shell/script đọc được số này (echo $?) để biết chương trình báo lỗi */
  }
  printf("Binh thuong -> main tra ve 0\n");
  return 0;
}
