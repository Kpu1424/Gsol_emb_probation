/*
 * Ý chính: ĐỊNH NGHĨA biến toàn cục g_temp_limit: cấp bộ nhớ, chỉ được có đúng 1 lần trong cả chương trình.
 * Build:   (xem main.c)
 * Quan sát: main.c đọc được biến này nhờ khai báo extern trong config.h.
 */
#include "config.h"

int g_temp_limit = 40;   /* định nghĩa: nếu đặt dòng này trong .h sẽ bị "multiple definition" khi nhiều .c include */

void config_set_limit(int limit) {
  g_temp_limit = limit;
}
