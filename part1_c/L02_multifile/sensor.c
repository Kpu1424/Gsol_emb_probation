/*
 * sensor.c - CÀI ĐẶT (implementation) của module cảm biến.
 */
#include <stdio.h>
#include "sensor.h"   /* include header của chính mình: compiler kiểm tra khớp khai báo */

/* ĐỊNH NGHĨA biến toàn cục: chỉ được có ĐÚNG MỘT lần trong toàn chương trình */
int g_sensor_read_count = 0;

/* static ở mức file: chỉ nhìn thấy trong sensor.c (ẩn khỏi main.c) */
static int s_base_raw[SENSOR_MAX_CHANNELS];

/* Hàm static: hàm "nội bộ", file khác không gọi được */
static int is_valid_channel(int channel) {
  return channel >= 0 && channel < SENSOR_MAX_CHANNELS;
}

void sensor_init(void) {
  for (int ch = 0; ch < SENSOR_MAX_CHANNELS; ch++) {
    s_base_raw[ch] = 200 + ch * 25;   /* giá trị giả lập: 20.0, 22.5, 25.0, 27.5 C */
  }
  printf("[sensor] init %d kenh\n", SENSOR_MAX_CHANNELS);
}

int sensor_read_raw(int channel) {
  if (!is_valid_channel(channel)) {
    return -1;
  }
  g_sensor_read_count++;
  return s_base_raw[channel] + g_sensor_read_count;  /* thay đổi nhẹ mỗi lần đọc */
}

float sensor_raw_to_celsius(int raw) {
  return raw / 10.0f;
}
