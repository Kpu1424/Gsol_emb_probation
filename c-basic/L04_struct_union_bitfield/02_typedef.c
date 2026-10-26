/*
 * Ý chính: typedef đặt TÊN MỚI cho một kiểu đã có, ví dụ "sensor_t" thay cho "struct sensor".
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 02_typedef.c -o 02_typedef
 * Quan sát: a (struct sensor) và b (sensor_t) là cùng một kiểu nên "b = a" chép được; millivolt_t có sizeof bằng int.
 */
#include <stdio.h>

struct sensor {
  int id;
  int temp;
};

typedef struct sensor sensor_t;   /* từ đây viết sensor_t cũng chính là struct sensor */
typedef int millivolt_t;          /* typedef cho kiểu cơ bản: tên nói rõ đơn vị, kiểu vẫn là int */

int main(void) {
  struct sensor a = {1, 25};
  sensor_t b;
  millivolt_t v = 3300;

  b = a;   /* cùng kiểu nên gán trực tiếp được: chép toàn bộ thành viên */
  printf("a: id = %d, temp = %d\n", a.id, a.temp);
  printf("b: id = %d, temp = %d (chep tu a)\n", b.id, b.temp);
  printf("v = %d mV, sizeof(millivolt_t) = %zu, sizeof(int) = %zu\n", v, sizeof(millivolt_t), sizeof(int));
  return 0;
}
