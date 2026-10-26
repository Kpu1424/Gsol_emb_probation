/*
 * Bài 04b - union
 * Ý chính: mọi thành viên của union DÙNG CHUNG một vùng nhớ (cùng bắt đầu ở offset 0).
 *          Ghi vào thành viên này sẽ làm thay đổi cách đọc thành viên khác.
 *
 * Build: make   Chạy: ./b_union
 */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

/* Một "từ" 4 byte nhận từ cảm biến, có thể nhìn theo 3 cách */
typedef union {
  uint32_t u32;
  float    f32;        /* thường là 4 byte, nhưng chuẩn C không bắt buộc */
  uint8_t  bytes[4];
} raw_word_t;

/* So sánh: struct có CÙNG các thành viên nhưng LƯU RIÊNG */
typedef struct {
  uint32_t u32;
  float    f32;
  uint8_t  bytes[4];
} raw_word_struct_t;

static void dump_bytes(const raw_word_t *w) {
  printf("  bytes[0..3] = %02X %02X %02X %02X\n",
         w->bytes[0], w->bytes[1], w->bytes[2], w->bytes[3]);
}

/* ===== Ứng dụng thực tế: tagged union ===== */
typedef enum { VALUE_INT, VALUE_FLOAT } value_kind_t;

typedef struct {
  value_kind_t kind;     /* "tag": cho biết thành viên nào trong union đang hợp lệ */
  union {
    int32_t i;
    float   f;
  } as;
} sensor_value_t;

static void print_value(const sensor_value_t *v) {
  if (v->kind == VALUE_INT) printf("  INT   %d\n", (int)v->as.i);
  else                      printf("  FLOAT %.2f\n", v->as.f);
}

int main(void) {
  raw_word_t w;

  printf("== 1. Tat ca thanh vien bat dau o CUNG dia chi ==\n");
  printf("  &w.u32   = %p (offset %zu)\n", (void *)&w.u32, offsetof(raw_word_t, u32));
  printf("  &w.f32   = %p (offset %zu)\n", (void *)&w.f32, offsetof(raw_word_t, f32));
  printf("  &w.bytes = %p (offset %zu)\n", (void *)&w.bytes, offsetof(raw_word_t, bytes));

  printf("\n== 2. sizeof: union vs struct cung thanh vien ==\n");
  printf("  sizeof(uint32_t)=%zu sizeof(float)=%zu sizeof(uint8_t[4])=%zu\n",
         sizeof(uint32_t), sizeof(float), sizeof(uint8_t[4]));
  printf("  sizeof(union)  = %zu  (>= thanh vien lon nhat)\n", sizeof(raw_word_t));
  printf("  sizeof(struct) = %zu  (>= tong cac thanh vien)\n", sizeof(raw_word_struct_t));

  printf("\n== 3. Ghi u32 roi doc bytes: thay doi thanh vien nay -> thay doi cach doc thanh vien kia ==\n");
  w.u32 = 0x11223344u;
  printf("  w.u32 = 0x%08X\n", (unsigned)w.u32);
  dump_bytes(&w);
  /* Thứ tự byte (endianness) phụ thuộc kiến trúc: kiểm tra lúc chạy, không giả định */
  if (w.bytes[0] == 0x44)      printf("  -> May NAY la little-endian (byte thap nam truoc)\n");
  else if (w.bytes[0] == 0x11) printf("  -> May NAY la big-endian (byte cao nam truoc)\n");
  else                         printf("  -> Thu tu byte khac thuong gap\n");

  printf("\n== 4. Sua 1 byte -> u32 cung doi ==\n");
  w.bytes[0] = 0xFF;
  printf("  sau w.bytes[0] = 0xFF: w.u32 = 0x%08X\n", (unsigned)w.u32);

  printf("\n== 5. Ghi f32 -> u32 cu bi GHI DE ==\n");
  w.f32 = 25.5f;
  printf("  w.f32 = %.2f, doc w.u32 = 0x%08X (bit bieu dien float tren may nay)\n",
         w.f32, (unsigned)w.u32);
  dump_bytes(&w);

  printf("\n== 6. Struct: sua 1 thanh vien KHONG anh huong thanh vien khac ==\n");
  raw_word_struct_t s = { .u32 = 0x11223344u, .f32 = 0.0f, .bytes = {0} };
  s.f32 = 25.5f;
  printf("  s.u32 = 0x%08X (van giu nguyen), s.f32 = %.2f\n", (unsigned)s.u32, s.f32);

  printf("\n== 7. Tagged union: dung tag de biet doc thanh vien nao ==\n");
  sensor_value_t a = { .kind = VALUE_INT,   .as.i = 1013 };
  sensor_value_t b = { .kind = VALUE_FLOAT, .as.f = 25.3f };
  print_value(&a);
  print_value(&b);
  printf("  sizeof(sensor_value_t) = %zu\n", sizeof(sensor_value_t));

  return 0;
}
