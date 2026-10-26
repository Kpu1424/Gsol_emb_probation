/*
 * Bài 04d - So sánh sizeof: struct vs union vs bit field, và ảnh hưởng của thứ tự thành viên
 *
 * Build: make   Chạy: ./d_compare_sizeof
 */
#include <stdio.h>
#include <stdint.h>

/* Cùng 3 thành viên, khác thứ tự -> có thể khác padding */
typedef struct { uint8_t a; uint32_t b; uint8_t c; } order_bad_t;
typedef struct { uint32_t b; uint8_t a; uint8_t c; } order_good_t;

typedef union  { uint8_t a; uint32_t b; uint8_t c; } same_union_t;

typedef struct { unsigned int a : 1; unsigned int b : 4; unsigned int c : 3; } bits_t;

/* Kết hợp hay gặp trong nhúng: union của bit field và số nguyên thô.
 * Tiện để xem, nhưng KHÔNG portable: vị trí bit phụ thuộc compiler. */
typedef union {
  bits_t  bits;
  uint32_t raw;
} bits_view_t;

int main(void) {
  printf("%-30s %s\n", "Kieu", "sizeof (tren may nay)");
  printf("%-30s %zu\n", "uint8_t + uint32_t + uint8_t", sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint8_t));
  printf("%-30s %zu\n", "struct {u8 a; u32 b; u8 c;}", sizeof(order_bad_t));
  printf("%-30s %zu\n", "struct {u32 b; u8 a; u8 c;}", sizeof(order_good_t));
  printf("%-30s %zu\n", "union  {u8 a; u32 b; u8 c;}", sizeof(same_union_t));
  printf("%-30s %zu\n", "bit field (1+4+3 bit)", sizeof(bits_t));

  bits_view_t v = { .raw = 0 };
  v.bits.a = 1;
  v.bits.b = 0xA;
  v.bits.c = 5;
  printf("\nbits a=1 b=0xA c=5 -> raw = 0x%08X\n", (unsigned)v.raw);
  printf("(Gia tri raw nay chi dung cho compiler + kien truc hien tai.)\n");
  return 0;
}
