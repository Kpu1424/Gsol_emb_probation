/*
 * sensor.h - GIAO DIỆN (interface) của module cảm biến.
 * Header chỉ chứa KHAI BÁO: "có những gì", không chứa "làm thế nào".
 */
#ifndef SENSOR_H   /* include guard: tránh bị include 2 lần trong 1 file .c */
#define SENSOR_H

#define SENSOR_MAX_CHANNELS 4

/* extern = KHAI BÁO biến toàn cục; ĐỊNH NGHĨA (cấp bộ nhớ) nằm trong sensor.c */
extern int g_sensor_read_count;

/* Khai báo hàm (prototype) */
void sensor_init(void);
int  sensor_read_raw(int channel);      /* trả về -1 nếu channel sai */
float sensor_raw_to_celsius(int raw);

#endif /* SENSOR_H */
