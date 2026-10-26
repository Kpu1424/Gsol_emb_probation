/*
 * Ý chính: Header (.h) chỉ chứa KHAI BÁO: "có hàm này", không chứa thân hàm.
 * Build:   (xem main.c)
 * Quan sát: file .c nào muốn gọi sensor_read_temp thì #include "sensor.h".
 */
#ifndef SENSOR_H   /* include guard: nếu một file .c lỡ include 2 lần thì lần sau bị bỏ qua */
#define SENSOR_H

int sensor_read_temp(void);

#endif
