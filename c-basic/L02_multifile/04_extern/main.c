/*
 * Ý chính: Nhờ extern, main.c và config.c cùng dùng MỘT biến g_temp_limit duy nhất.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 main.c config.c -o extern_app
 * Quan sát: config.c đổi giá trị thành 50, main.c đọc lại thấy 50 ngay: không có bản sao nào.
 */
#include <stdio.h>
#include "config.h"

int main(void) {
  printf("main.c doc g_temp_limit = %d\n", g_temp_limit);

  config_set_limit(50);   /* hàm trong config.c sửa biến */
  printf("Sau config_set_limit(50), main.c doc lai g_temp_limit = %d\n", g_temp_limit);
  return 0;
}
