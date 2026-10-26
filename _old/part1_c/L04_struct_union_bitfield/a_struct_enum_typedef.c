/*
 * Bài 04a - struct, enum, typedef
 * Ý chính: các thành viên của struct được LƯU RIÊNG (mỗi thành viên có vùng nhớ riêng),
 *          sửa thành viên này KHÔNG ảnh hưởng thành viên khác.
 *
 * Build: make   Chạy: ./a_struct_enum_typedef
 */
#include <stdio.h>
#include <stddef.h>   /* offsetof */
#include <stdint.h>   /* uint8_t, int16_t, uint32_t: kiểu có độ rộng cố định */

/* enum: đặt TÊN cho các hằng số nguyên. Mặc định bắt đầu từ 0, tăng dần. */
typedef enum {
  SENSOR_TEMP = 0,
  SENSOR_HUMI,      /* = 1 */
  SENSOR_PRESS      /* = 2 */
} sensor_type_t;

/* struct: gom nhiều dữ liệu KHÁC KIỂU thành 1 bản ghi.
 * typedef: đặt tên ngắn "sensor_reading_t" thay vì phải viết "struct sensor_reading". */
typedef struct sensor_reading {
  uint8_t       id;            /* 1 byte */
  sensor_type_t type;          /* kiểu enum, kích thước do compiler quyết định */
  int16_t       value_x10;     /* giá trị x10: 253 = 25.3 */
  uint32_t      timestamp_ms;
} sensor_reading_t;

const char *type_name(sensor_type_t t) {
  switch (t) {
    case SENSOR_TEMP:  return "TEMP";
    case SENSOR_HUMI:  return "HUMI";
    case SENSOR_PRESS: return "PRESS";
  }
  return "UNKNOWN";
}

void print_reading(const sensor_reading_t *r) {   /* truyền con trỏ: không copy cả struct */
  printf("  id=%u type=%s value=%d.%d ts=%u ms\n",
         (unsigned)r->id, type_name(r->type),       /* r->x  tương đương (*r).x */
         r->value_x10 / 10, r->value_x10 % 10, (unsigned)r->timestamp_ms);
}

void calibrate(sensor_reading_t *r, int16_t offset_x10) {
  r->value_x10 += offset_x10;   /* sửa struct GỐC của người gọi qua con trỏ */
}

int main(void) {
  /* Khởi tạo có tên trường (designated initializer) - rõ ràng, ít nhầm thứ tự */
  sensor_reading_t r = { .id = 1, .type = SENSOR_TEMP, .value_x10 = 253, .timestamp_ms = 1000 };

  printf("== 1. Gia tri ban dau ==\n");
  print_reading(&r);

  printf("\n== 2. Moi thanh vien co vi tri (offset) RIENG ==\n");
  printf("  offsetof id           = %zu, sizeof = %zu\n", offsetof(sensor_reading_t, id), sizeof(r.id));
  printf("  offsetof type         = %zu, sizeof = %zu\n", offsetof(sensor_reading_t, type), sizeof(r.type));
  printf("  offsetof value_x10    = %zu, sizeof = %zu\n", offsetof(sensor_reading_t, value_x10), sizeof(r.value_x10));
  printf("  offsetof timestamp_ms = %zu, sizeof = %zu\n", offsetof(sensor_reading_t, timestamp_ms), sizeof(r.timestamp_ms));
  size_t sum = sizeof(r.id) + sizeof(r.type) + sizeof(r.value_x10) + sizeof(r.timestamp_ms);
  printf("  Tong sizeof cac thanh vien = %zu, sizeof(struct) = %zu\n", sum, sizeof(r));
  printf("  -> Phan chenh lech (neu co) la PADDING compiler chen vao de can le (alignment).\n");
  printf("     Gia tri cu the PHU THUOC compiler/kien truc, khong co dinh tren moi he thong.\n");

  printf("\n== 3. Sua 1 thanh vien, cac thanh vien khac giu nguyen ==\n");
  r.value_x10 = 300;
  print_reading(&r);

  printf("\n== 4. Gan struct = COPY toan bo, 2 ban doc lap ==\n");
  sensor_reading_t copy = r;
  copy.id = 2;
  copy.type = SENSOR_HUMI;
  printf("  r   :"); print_reading(&r);
  printf("  copy:"); print_reading(&copy);

  printf("\n== 5. Truyen con tro vao ham de sua struct goc ==\n");
  calibrate(&r, -15);
  print_reading(&r);

  printf("\n== 6. enum la so nguyen ==\n");
  printf("  SENSOR_TEMP=%d SENSOR_HUMI=%d SENSOR_PRESS=%d, sizeof(sensor_type_t)=%zu\n",
         SENSOR_TEMP, SENSOR_HUMI, SENSOR_PRESS, sizeof(sensor_type_t));

  return 0;
}
