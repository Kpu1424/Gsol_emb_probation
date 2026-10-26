/*
 * Ý chính: Với con trỏ tới struct, p->temp là cách viết gọn của (*p).temp; truyền con trỏ để hàm sửa được struct gốc.
 * Build:   gcc -std=c11 -Wall -Wextra -g -O0 07_struct_pointer_arrow.c -o 07_struct_pointer_arrow
 * Quan sát: 3 cách đọc đều ra 25; sau calibrate(&s, -2) thì s.temp trong main thành 23.
 */
#include <stdio.h>

typedef struct {
  int id;
  int temp;
} sensor_t;

void calibrate(sensor_t *p, int offset) {
  p->temp = p->temp + offset;   /* sửa struct GỐC qua địa chỉ, không phải bản sao */
}

int main(void) {
  sensor_t s = {1, 25};
  sensor_t *p = &s;

  printf("s.temp    = %d\n", s.temp);
  printf("(*p).temp = %d\n", (*p).temp);   /* cần ngoặc: *p.temp sẽ bị hiểu là *(p.temp) */
  printf("p->temp   = %d\n", p->temp);

  calibrate(&s, -2);
  printf("Sau calibrate(&s, -2): s.temp = %d\n", s.temp);
  return 0;
}
