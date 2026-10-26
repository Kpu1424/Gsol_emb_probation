/*
 * Ý chính: main.c chỉ cần #include "sensor.h" để gọi hàm định nghĩa ở sensor.c; bước link nối 2 file lại.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 main.c sensor.c -o sensor_app
 * Quan sát: "Nhiet do doc tu sensor.c: 25 C".
 */
#include <stdio.h>
#include "sensor.h"   /* dấu "" : tìm file trong thư mục hiện tại trước */

int main(void) {
  int t = sensor_read_temp();   /* compiler chỉ cần khai báo; thân hàm do linker tìm trong sensor.o */
  printf("Nhiet do doc tu sensor.c: %d C\n", t);
  return 0;
}
