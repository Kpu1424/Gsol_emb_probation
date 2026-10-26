/*
 * Bài 04c - bit field
 * Ý chính: nhiều trường nhỏ (vài bit) được "đóng gói" chung trong một đơn vị lưu trữ.
 *          Giá trị vượt số bit sẽ bị CẮT. Vị trí/thứ tự bit do COMPILER quyết định.
 *
 * Build: make   Chạy: ./c_bitfield
 */
#include <stdio.h>
#include <string.h>   /* memcpy */

/* Thanh ghi trạng thái cảm biến (giả lập), dùng bit field */
typedef struct {
  unsigned int enabled : 1;   /* 0..1  */
  unsigned int error   : 1;   /* 0..1  */
  unsigned int mode    : 2;   /* 0..3  */
  unsigned int channel : 4;   /* 0..15 */
} status_bits_t;

/* Cùng ý nghĩa nhưng mỗi trường là 1 unsigned int đầy đủ */
typedef struct {
  unsigned int enabled;
  unsigned int error;
  unsigned int mode;
  unsigned int channel;
} status_plain_t;

/* Cách PORTABLE thường dùng cho thanh ghi phần cứng / giao thức: mask + shift tự định nghĩa */
#define ST_ENABLED_MASK  (1u << 0)
#define ST_ERROR_MASK    (1u << 1)
#define ST_MODE_SHIFT    2
#define ST_MODE_MASK     (0x3u << ST_MODE_SHIFT)
#define ST_CHANNEL_SHIFT 4
#define ST_CHANNEL_MASK  (0xFu << ST_CHANNEL_SHIFT)

static void print_bits(const char *title, const status_bits_t *s) {
  printf("  %-28s enabled=%u error=%u mode=%u channel=%u\n",
         title, s->enabled, s->error, s->mode, s->channel);
}

/* Xem các byte thực sự trong bộ nhớ (chỉ để QUAN SÁT trên máy này) */
static void dump_raw(const status_bits_t *s) {
  unsigned char raw[sizeof(status_bits_t)];
  memcpy(raw, s, sizeof raw);
  printf("  raw bytes:");
  for (size_t i = 0; i < sizeof raw; i++) printf(" %02X", raw[i]);
  printf("\n");
}

int main(void) {
  status_bits_t s = {0};   /* xóa hết về 0 */

  printf("== 1. sizeof: bit field vs struct thuong ==\n");
  printf("  sizeof(status_bits_t)  = %zu\n", sizeof(status_bits_t));
  printf("  sizeof(status_plain_t) = %zu\n", sizeof(status_plain_t));
  printf("  -> 8 bit thong tin, nhung kich thuoc that do compiler chon (thuong theo unsigned int).\n");

  printf("\n== 2. Gan gia tri hop le ==\n");
  s.enabled = 1;
  s.mode = 2;
  s.channel = 9;
  print_bits("sau khi gan:", &s);
  dump_raw(&s);

  printf("\n== 3. Sua 1 truong, cac truong khac giu nguyen ==\n");
  s.error = 1;
  print_bits("sau s.error = 1:", &s);
  dump_raw(&s);
  printf("  -> Cac truong DUNG CHUNG cac byte tren, nhung moi truong co nhom bit RIENG.\n");

  printf("\n== 4. Gia tri vuot qua so bit bi CAT (khong bao loi luc chay) ==\n");
  unsigned int requested_mode = 5;      /* 5 = 0b101, mode chỉ có 2 bit */
  s.mode = requested_mode;              /* giữ 2 bit thấp: 0b01 = 1 */
  printf("  yeu cau mode=%u, doc lai mode=%u\n", requested_mode, s.mode);
  unsigned int requested_ch = 20;       /* 20 = 0b10100, channel chỉ có 4 bit */
  s.channel = requested_ch;
  printf("  yeu cau channel=%u, doc lai channel=%u\n", requested_ch, s.channel);

  printf("\n== 5. Cach portable: mask/shift tren 1 so nguyen ==\n");
  unsigned int reg = 0;
  reg |= ST_ENABLED_MASK;                                         /* bật bit */
  reg = (reg & ~ST_MODE_MASK)    | ((2u << ST_MODE_SHIFT)    & ST_MODE_MASK);
  reg = (reg & ~ST_CHANNEL_MASK) | ((9u << ST_CHANNEL_SHIFT) & ST_CHANNEL_MASK);
  printf("  reg = 0x%02X\n", reg);
  printf("  enabled=%u error=%u mode=%u channel=%u\n",
         (reg & ST_ENABLED_MASK) ? 1u : 0u,
         (reg & ST_ERROR_MASK) ? 1u : 0u,
         (reg & ST_MODE_MASK) >> ST_MODE_SHIFT,
         (reg & ST_CHANNEL_MASK) >> ST_CHANNEL_SHIFT);
  printf("  -> Vi tri bit o day do CHINH TA quy dinh, giong nhau tren moi compiler.\n");

  /* Lỗi thường gặp: KHÔNG lấy được địa chỉ bit field
   *   unsigned int *p = &s.mode;   // lỗi biên dịch: cannot take address of bit-field
   */
  return 0;
}
