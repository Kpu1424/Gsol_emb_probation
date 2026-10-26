/*
 * Ý chính: static ở mức file: biến count chỉ nhìn thấy trong counter.c, file khác không truy cập trực tiếp được.
 * Build:   (xem main_right.c, main_wrong.c)
 * Quan sát: main_right.c dùng được count qua hàm; main_wrong.c dùng trực tiếp thì lỗi link.
 */
#include "counter.h"

static int count = 0;   /* static: tên count không được đưa ra cho linker, nên file khác không "thấy" nó */

void counter_increment(void) {
  count++;
}

int counter_get(void) {
  return count;
}
