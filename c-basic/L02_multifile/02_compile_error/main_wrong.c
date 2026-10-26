/*
 * Ý chính: (FILE SAI CỐ Ý) Dùng biến temp_limit chưa hề được khai báo -> gcc
 * dừng ở bước COMPILE, chưa tới bước link. Build:   gcc -std=c11 -Wall -Wextra
 * -g -O0 -c main_wrong.c -o main_wrong.o Quan sát: "error: 'temp_limit'
 * undeclared": lỗi compile luôn chỉ ra tên file .c, số dòng và cột.
 */
#include "../01_basic/sensor.h"
#include <stdio.h>

int main(void) {
  int t = sensor_read_temp();
  int temp_limit = 28;
  /* temp_limit không được khai báo ở đâu cả (ví dụ: quên #include header có
   * khai báo nó) */
  if (t > temp_limit) {
    printf("Qua nhiet\n");
  }
  return 0;
}
