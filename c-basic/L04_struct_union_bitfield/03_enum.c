/*
 * Ý chính: enum đặt tên cho một nhóm hằng số nguyên; mặc định bắt đầu từ 0 và tăng dần 1.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 03_enum.c -o 03_enum
 * Quan sát: MODE_OFF = 0, MODE_NORMAL = 1, MODE_FAST = 2; switch dùng tên dễ đọc thay cho số 0, 1, 2.
 */
#include <stdio.h>

typedef enum {
  MODE_OFF,      /* 0 */
  MODE_NORMAL,   /* 1 */
  MODE_FAST      /* 2 */
} sensor_mode_t;

int main(void) {
  sensor_mode_t mode = MODE_FAST;

  printf("MODE_OFF = %d, MODE_NORMAL = %d, MODE_FAST = %d\n", MODE_OFF, MODE_NORMAL, MODE_FAST);
  printf("mode hien tai = %d\n", mode);

  switch (mode) {   /* -Wall cảnh báo nếu thiếu case cho một giá trị của enum */
    case MODE_OFF:
      printf("Cam bien dang tat\n");
      break;
    case MODE_NORMAL:
      printf("Doc 1 lan moi giay\n");
      break;
    case MODE_FAST:
      printf("Doc 10 lan moi giay\n");
      break;
  }
  return 0;
}
