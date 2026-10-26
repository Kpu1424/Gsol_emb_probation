/*
 * Ý chính: struct gom nhiều biến liên quan thành một bản ghi; mỗi thành viên có vùng nhớ riêng.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 01_struct_basic.c -o 01_struct_basic
 * Quan sát: sửa s.temp thành 30 thì s.id và s.voltage vẫn giữ nguyên.
 */
#include <stdio.h>

struct sensor {
  int id;
  int temp;      /* độ C */
  int voltage;   /* mV */
};

int main(void) {
  struct sensor s = {1, 25, 3300};   /* khởi tạo theo đúng thứ tự khai báo: id, temp, voltage */

  printf("Ban dau:          id = %d, temp = %d, voltage = %d\n", s.id, s.temp, s.voltage);
  s.temp = 30;                       /* dấu chấm: truy cập một thành viên */
  printf("Sau khi sua temp: id = %d, temp = %d, voltage = %d\n", s.id, s.temp, s.voltage);
  printf("sizeof(struct sensor) = %zu byte (3 thanh vien int x 4 byte)\n", sizeof(struct sensor));
  return 0;
}
