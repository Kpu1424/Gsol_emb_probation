/*
 * Ý chính: File .c chứa ĐỊNH NGHĨA (thân hàm) cho những gì sensor.h đã khai báo.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 -c sensor.c -o sensor.o   (chỉ compile, chưa link)
 * Quan sát: file này không chạy riêng được vì không có main; xem main.c.
 */
#include "sensor.h"   /* include header của chính mình: compiler kiểm tra định nghĩa khớp khai báo */

int sensor_read_temp(void) {
  return 25;   /* giả lập cảm biến: luôn đọc được 25 độ C */
}
