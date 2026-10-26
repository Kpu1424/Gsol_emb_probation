/*
 * main.c - Dùng module sensor qua header, không cần biết bên trong.
 * Build: make   Chạy: ./sensor_app
 */
#include <stdio.h>
#include "sensor.h"   /* "..." : tìm trong thư mục hiện tại trước */

int main(void) {
  sensor_init();

  for (int ch = 0; ch <= SENSOR_MAX_CHANNELS; ch++) {  /* cố ý đọc cả kênh không hợp lệ */
    int raw = sensor_read_raw(ch);
    if (raw < 0) {
      printf("Kenh %d: loi (kenh khong ton tai)\n", ch);
      continue;
    }
    printf("Kenh %d: raw=%d -> %.1f C\n", ch, raw, sensor_raw_to_celsius(raw));
  }

  /* Đọc biến toàn cục định nghĩa ở sensor.c nhờ khai báo extern trong sensor.h */
  printf("Tong so lan doc thanh cong: %d\n", g_sensor_read_count);

  /* s_base_raw và is_valid_channel là static trong sensor.c -> không dùng được ở đây */
  return 0;
}
